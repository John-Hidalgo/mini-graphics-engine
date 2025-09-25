#include "Toolbar.h"

Toolbar::Toolbar() : canvasRef(nullptr) {}

void Toolbar::setup(Canvas* canvas) {
	canvasRef = canvas;
	
	dessinez.setup("Dessinez");
	dessinez.setPosition(220,0);
	
	dessinez.add(rectangleToggle.setup("Rectangle", false));
	rectangleToggle.addListener(this, &Toolbar::rectangleToggleChanged);
	
	dessinez.add(circleToggle.setup("Cercle", false));
	circleToggle.addListener(this, &Toolbar::circleToggleChanged);
	
	dessinez.add(lineToggle.setup("Ligne", false));
	lineToggle.addListener(this, &Toolbar::lineToggleChanged);
	
	dessinez.add(freeformToggle.setup("Libre", false));
	freeformToggle.addListener(this, &Toolbar::freeformToggleChanged);
	
	dessinez.add(selectColourToggle.setup("coleur du canevas", false));
	selectColourToggle.addListener(this, &Toolbar::selectColourToggleChanged);
	
	dessinez.add(colorSlider.setup("Couleur", ofColor(0, 0, 0), ofColor(0,0,0), ofColor(255,255,255)));
	static_cast<ofParameter<ofColor>&>(colorSlider.getParameter()).addListener(this, &Toolbar::colorChanged);

	dessinez.add(undoButton.setup("defaire"));
	undoButton.addListener(this, &Toolbar::undoButtonPressed);

	dessinez.minimize();
	
	importation.setup("Importation");
	importation.setPosition(0,0);

	importation.add(importImageButton.setup("Importez Image"));
	importImageButton.addListener(this, &Toolbar::importImagePressed);

	importation.add(importModelButton.setup("Importez Model 3D"));
	importModelButton.addListener(this, &Toolbar::importModelPressed);
	
	importation.minimize();

	echantillonage.setup("Echantillonage");
	echantillonage.setPosition(440,0);

	echantillonage.add(echantillon1Button.setup("Échantillonez une Image"));
	echantillon1Button.addListener(this, &Toolbar::echantillon1Pressed);

	echantillonage.add(echantillon2Button.setup("Échantillonez une Image"));
	echantillon2Button.addListener(this, &Toolbar::echantillon2Pressed);

	echantillonage.minimize();
}

void Toolbar::draw() {
	dessinez.draw();
	importation.draw();
	echantillonage.draw();
}

void Toolbar::rectangleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::RECTANGLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::RECTANGLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}
}

void Toolbar::circleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::CIRCLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::CIRCLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}
}

void Toolbar::lineToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::LINE);
	else if (canvasRef->getCurrentMode() == ShapeMode::LINE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}
}

void Toolbar::freeformToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::FREEFORM);
	else if (canvasRef->getCurrentMode() == ShapeMode::FREEFORM) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}
}

void Toolbar::selectColourToggleChanged(bool & val) {
	pickingColour = val;

	if (val) {
		rectangleToggle = false;
		circleToggle    = false;
		lineToggle      = false;
		freeformToggle  = false;
		if (canvasRef) canvasRef->setCurrentMode(ShapeMode::NONE);
	}
}


void Toolbar::undoButtonPressed() {
	if (canvasRef) {
		canvasRef->undo();
	}
}
void Toolbar::importImagePressed() {
	ofFileDialogResult result = ofSystemLoadDialog("Choissisez un image");
	if (result.bSuccess) {
		std::string path = result.getPath();
		if (canvasRef) {
			canvasRef->loadImage(path);
		}
	}
}

void Toolbar::setExclusiveToggle(ShapeMode mode) {
	rectangleToggle = (mode == ShapeMode::RECTANGLE);
	circleToggle    = (mode == ShapeMode::CIRCLE);
	lineToggle      = (mode == ShapeMode::LINE);
	freeformToggle  = (mode == ShapeMode::FREEFORM);
	selectColourToggle = (mode == ShapeMode::NONE && pickingColour);

	if (canvasRef) {
		if (mode == ShapeMode::NONE && pickingColour) {
			canvasRef->setCurrentMode(ShapeMode::NONE);
		} else {
			canvasRef->setCurrentMode(mode);
			canvasRef->setDrawingColor(colorSlider);
		}
	}
}
void Toolbar::colorChanged(ofColor& col) {
	if (canvasRef) {
		canvasRef->setDrawingColor(col);
	}
}

void Toolbar::importModelPressed() {
	std::string defaultPath = ofToDataPath("", true);
	ofFileDialogResult result = ofSystemLoadDialog(
		"Select a 3D model (.obj)",
		false,
		defaultPath
	);

	if(result.bSuccess){
		std::string filepath = result.getPath();
		ofLogNotice() << "Loaded model: " << filepath;
		canvasRef->loadModel(filepath);
	}
}


void Toolbar::echantillon1Pressed() {
	std::cout << "echantillon1Pressed!" << "\n";

	// On demande au user de choisir une image
	ofFileDialogResult result = ofSystemLoadDialog("Choisissez une image à échantillonner");
	if (!result.bSuccess) return;

	// On charge l'image source
	ofImage source;
	if (!source.load(result.getPath())) {
		ofLogError() << "Impossible de charger l'image : " << result.getPath();
		return;
	}

	// dimensions d'échantillon
	int sampleWidth  = source.getWidth()  / 3;
	int sampleHeight = source.getHeight() / 3;

	// On crée une nouvelle image de destination
	ofImage destination;
	destination.allocate(sampleWidth * 2, sampleHeight * 2, OF_IMAGE_COLOR);

	// 4 échantillons de l'image source
	ofImage sample1, sample2, sample3, sample4;
	sample1.cropFrom(source, 0, 0, sampleWidth, sampleHeight);
	sample2.cropFrom(source, sampleWidth, 0, sampleWidth, sampleHeight);
	sample3.cropFrom(source, 0, sampleHeight, sampleWidth, sampleHeight);
	sample4.cropFrom(source, sampleWidth, sampleHeight, sampleWidth, sampleHeight);

	// On dessine ces échantillons dans une fbo temporaire
	ofFbo fbo;
	fbo.allocate(destination.getWidth(), destination.getHeight(), GL_RGBA);
	fbo.begin();
	ofClear(0,0,0,0);

	sample1.draw(0, 0, sampleWidth, sampleHeight);
	sample2.draw(sampleWidth, 0, sampleWidth, sampleHeight);
	sample3.draw(0, sampleHeight, sampleWidth, sampleHeight);
	sample4.draw(sampleWidth, sampleHeight, sampleWidth, sampleHeight);

	fbo.end();

	// On cope le rendu du fbo vers destination
	fbo.readToPixels(destination.getPixels());
	destination.update();

	// On sauvegarde l'image générée
	destination.save("echantillon.png");
	ofLogNotice() << "Image échantillonnée sauvegardée sous : data/echantillon.png";

	// Possiblement envoyer l'image dans le canvas
	if (canvasRef) {
		canvasRef->loadImage("echantillon.png");
	}
}


void Toolbar::echantillon2Pressed() {
	std::cout << "echantillon2Pressed!" << "\n";
}