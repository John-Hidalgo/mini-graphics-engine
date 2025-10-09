#include "Toolbar.h"

Toolbar::Toolbar() : canvasRef(nullptr) {}

void Toolbar::setup(Canvas* canvas) {
	canvasRef = canvas;

	// On charge les possibles curseurs au debut
	loadCursorIcons();

	dessinez.setup("Dessinez");
	dessinez.setPosition(220,0);
	dessinez.setSize(200, 0);
	
	dessinez.add(rectangleToggle.setup("Rectangle", false));
	rectangleToggle.addListener(this, &Toolbar::rectangleToggleChanged);
	
	dessinez.add(circleToggle.setup("Cercle", false));
	circleToggle.addListener(this, &Toolbar::circleToggleChanged);
	
	dessinez.add(lineToggle.setup("Ligne", false));
	lineToggle.addListener(this, &Toolbar::lineToggleChanged);

	dessinez.add(pointToggle.setup("Point", false));
	pointToggle.addListener(this, &Toolbar::pointToggleChanged);

	dessinez.add(squareToggle.setup("Carré", false));
	squareToggle.addListener(this, &Toolbar::squareToggleChanged);

	dessinez.add(triangleToggle.setup("Triangle", false));
	triangleToggle.addListener(this, &Toolbar::triangleToggleChanged);
	
	dessinez.add(freeformToggle.setup("Libre", false));
	freeformToggle.addListener(this, &Toolbar::freeformToggleChanged);
	
	dessinez.add(selectColourToggle.setup("Couleur du canevas", false));

	selectColourToggle.addListener(this, &Toolbar::selectColourToggleChanged);
	
	dessinez.add(redBtn.setup("Rouge"));
	dessinez.add(greenBtn.setup("Vert"));
	dessinez.add(blueBtn.setup("Bleue"));
	dessinez.add(yellowBtn.setup("Jaune"));

	currentColour.setup("Couleur Courant", "");
	dessinez.add(&currentColour);
	currentColour.setBackgroundColor(ofColor(255, 255, 255));

	redBtn.setBackgroundColor(ofColor(150, 50, 50));
	greenBtn.setBackgroundColor(ofColor(50, 150, 50));
	blueBtn.setBackgroundColor(ofColor(50, 50, 150));
	yellowBtn.setBackgroundColor(ofColor(150, 150, 50));

	redBtn.setTextColor(ofColor(255, 200, 200));
	greenBtn.setTextColor(ofColor(200, 255, 200));
	blueBtn.setTextColor(ofColor(200, 200, 255));
	yellowBtn.setTextColor(ofColor(255, 255, 200));
	
	redBtn.addListener(this, &Toolbar::onRedPressed);
	greenBtn.addListener(this, &Toolbar::onGreenPressed);
	blueBtn.addListener(this, &Toolbar::onBluePressed);
	yellowBtn.addListener(this, &Toolbar::onYellowPressed);

	currentColor = ofColor(255, 255, 255);
	currentColorName = "";
	
	dessinez.add(undoButton.setup("defaire"));
	undoButton.addListener(this, &Toolbar::undoButtonPressed);
	
	dessinez.add(histogramToggle.setup("Affichez l'histogramme", false));
	histogramToggle.addListener(this, &Toolbar::histogramToggleChanged);

	dessinez.minimize();

	echantillonage.setup("Echantillonage");
	echantillonage.setPosition(440,0);
	echantillonage.setSize(200,0);

	echantillonage.add(echantillon1Button.setup("Échantillonez une Image"));
	echantillon1Button.addListener(this, &Toolbar::echantillon1Pressed);

	echantillonage.add(echantillon2Button.setup("Échantillonez une Image"));
	echantillon2Button.addListener(this, &Toolbar::echantillon2Pressed);

	echantillonage.minimize();
	
	importation.setup("Importation");
	importation.setPosition(660,0);
	importation.setSize(200,0);

	importation.add(importImageButton.setup("Importez Image"));
	importImageButton.addListener(this, &Toolbar::importImagePressed);

	importation.add(importModelButton.setup("Importez Model 3D"));
	importModelButton.addListener(this, &Toolbar::importModelPressed);
	
	
	importation.minimize();

	// 3D Primitives Group
	primitives3DGroup.setup("Primitives 3D");
	primitives3DGroup.setPosition(880, 0);
	primitives3DGroup.setSize(200, 0);

	primitives3DGroup.add(sphereToggle.setup("Sphere", false));
	sphereToggle.addListener(this, &Toolbar::sphereToggleChanged);

	primitives3DGroup.add(cubeToggle.setup("Cube", false));
	cubeToggle.addListener(this, &Toolbar::cubeToggleChanged);

	primitives3DGroup.add(cylinderToggle.setup("Cylinder", false));
	cylinderToggle.addListener(this, &Toolbar::cylinderToggleChanged);

	primitives3DGroup.add(coneToggle.setup("Cone", false));
	coneToggle.addListener(this, &Toolbar::coneToggleChanged);

	primitives3DGroup.add(torusToggle.setup("Torus", false));
	torusToggle.addListener(this, &Toolbar::torusToggleChanged);

	primitives3DGroup.add(pyramidToggle.setup("Pyramid", false));
	pyramidToggle.addListener(this, &Toolbar::pyramidToggleChanged);

	primitives3DGroup.minimize();

	updateCursorIcon();
}

void Toolbar::draw() {
	dessinez.draw();
	importation.draw();
	echantillonage.draw();
	primitives3DGroup.draw();

	// Si on dessine une forme/primitive on draw le curseur
	if (isDrawingActive && selectedCursor.isAllocated()) {
		ofSetColor(255);
		selectedCursor.draw(ofGetMouseX() - 1.0f, ofGetMouseY(), 20, 20);
	}
}

void Toolbar::rectangleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::RECTANGLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::RECTANGLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::circleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::CIRCLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::CIRCLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::lineToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::LINE);
	else if (canvasRef->getCurrentMode() == ShapeMode::LINE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::pointToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::POINT);
	else if (canvasRef->getCurrentMode() == ShapeMode::POINT) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::squareToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::SQUARE);
	else if (canvasRef->getCurrentMode() == ShapeMode::SQUARE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::triangleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::TRIANGLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::TRIANGLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::freeformToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::FREEFORM);
	else if (canvasRef->getCurrentMode() == ShapeMode::FREEFORM) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::selectColourToggleChanged(bool & val) {
	pickingColour = val;

	if (val) {
		rectangleToggle = false;
		circleToggle    = false;
		lineToggle      = false;
		freeformToggle  = false;
		pointToggle = false;
		triangleToggle = false;
		squareToggle    = false;
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
	squareToggle    = (mode == ShapeMode::SQUARE);
	pointToggle     = (mode == ShapeMode::POINT);
	triangleToggle  = (mode == ShapeMode::TRIANGLE);
	freeformToggle  = (mode == ShapeMode::FREEFORM);
	selectColourToggle = false;

	// On désactive toutes toggle de primitives 3D primitive
	sphereToggle = false;
	cubeToggle = false;
	cylinderToggle = false;
	coneToggle = false;
	torusToggle = false;
	pyramidToggle = false;

	if (canvasRef) {
		canvasRef->setCurrentMode(mode);
		// Pour s'assurer que le mode 3D mode est désactivé
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
		canvasRef->setDrawingColor(currentColor);
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
	if (result.bSuccess) {
		auto newModel = std::make_unique<Model3D>();
		newModel->setup();
		newModel->loadModel(result.getPath());
		newModel->color_background = canvasRef->color_picker_background;
		newModel->color_ambient = canvasRef->color_picker_ambient;
		newModel->color_diffuse = canvasRef->color_picker_diffuse;
		canvasRef->models.push_back(std::move(newModel));
		ofLogNotice() << "Imported model: " << result.getPath();
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

void Toolbar::onRedPressed(){
	currentColor = ofColor(255, 0, 0);
	currentColour = "Rouge";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::onGreenPressed(){
	currentColor = ofColor(0, 255, 0);
	currentColour = "Vert";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::onBluePressed(){
	currentColor = ofColor(0, 0, 255);
	currentColour = "Bleue";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::onYellowPressed(){
	currentColor = ofColor(255, 255, 0);
	currentColour = "Jaune";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::setColorFromCanvas(ofColor& color,std::string& name){
	currentColour.setBackgroundColor(color);
	currentColour = name;
	currentColor = color;
		currentColour = name;
		currentColour.setBackgroundColor(currentColor);

		redBtn.setBackgroundColor(ofColor(150, 50, 50));
		greenBtn.setBackgroundColor(ofColor(50, 150, 50));
		blueBtn.setBackgroundColor(ofColor(50, 50, 150));
		yellowBtn.setBackgroundColor(ofColor(150, 150, 50));
		if (color == ofColor(255, 0, 0)) {
			redBtn.setBackgroundColor(ofColor(255, 100, 100));
		} else if (color == ofColor(0, 255, 0)) {
			greenBtn.setBackgroundColor(ofColor(100, 255, 100));
		} else if (color == ofColor(0, 0, 255)) {
			blueBtn.setBackgroundColor(ofColor(100, 100, 255));
		} else if (color == ofColor(255, 255, 0)) {
			yellowBtn.setBackgroundColor(ofColor(255, 255, 100));
		}
		
		if (canvasRef) {
			canvasRef->setDrawingColor(currentColor);
		}
};


void Toolbar::sphereToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::SPHERE);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::SPHERE) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::cubeToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::CUBE);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::CUBE) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::cylinderToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::CYLINDER);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::CYLINDER) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::coneToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::CONE);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::CONE) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::torusToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::TORUS);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::TORUS) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::pyramidToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::PYRAMID);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::PYRAMID) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::setExclusivePrimitiveToggle(Primitive3DType mode) {
	// On désactive toutes toggle de primitives 2D primitive
	rectangleToggle = false;
	circleToggle = false;
	lineToggle = false;
	freeformToggle = false;
	pointToggle = false;
	triangleToggle = false;
	squareToggle = false;
	selectColourToggle = false;

	// On active la bonne toggle sélectionné
	sphereToggle = (mode == Primitive3DType::SPHERE);
	cubeToggle = (mode == Primitive3DType::CUBE);
	cylinderToggle = (mode == Primitive3DType::CYLINDER);
	coneToggle = (mode == Primitive3DType::CONE);
	torusToggle = (mode == Primitive3DType::TORUS);
	pyramidToggle = (mode == Primitive3DType::PYRAMID);

	if (canvasRef) {
		canvasRef->setCurrentPrimitiveMode(mode);
		// Pour s'assurer que le mode 2D mode est désactivé
		canvasRef->setCurrentMode(ShapeMode::NONE);
  }
}

void Toolbar::histogramToggleChanged(bool& val) {
	if (canvasRef) {
		canvasRef->setShowHistogram(val);
	}
}

void Toolbar::loadCursorIcons() {
	std::filesystem::path cwd = std::filesystem::current_path();
	std::cout << "Current working directory: " << cwd << std::endl;

	std::string icons2DPath = cwd.string() + "/data/resources/icons/2D/";
	std::string icons3DPath = cwd.string() + "/data/resources/icons/3D/";

	// Shape 2D
	availableCursorIcons["rectangle"] = ofImage(icons2DPath + "rectangle.png");
	availableCursorIcons["circle"] = ofImage(icons2DPath + "circle.png");
	availableCursorIcons["line"] = ofImage(icons2DPath + "line1.png");
	availableCursorIcons["freeform"] = ofImage(icons2DPath + "pen1.png");
	availableCursorIcons["point"] = ofImage(icons2DPath + "point1.png");
	availableCursorIcons["square"] = ofImage(icons2DPath + "square.png");
	availableCursorIcons["triangle"] = ofImage(icons2DPath + "triangle.png");

	// Primitives 3D
	availableCursorIcons["sphere"] = ofImage(icons3DPath + "sphere1.png");
	availableCursorIcons["cube"] = ofImage(icons3DPath + "cube.png");
	availableCursorIcons["cylinder"] = ofImage(icons3DPath + "cylinder.png");
	availableCursorIcons["cone"] = ofImage(icons3DPath + "cone.png");
	availableCursorIcons["torus"] = ofImage(icons3DPath + "torus-2D.png");
	availableCursorIcons["pyramid"] = ofImage(icons3DPath + "pyramid-square-base.png");

	for (auto& cursor : availableCursorIcons) {
		cursor.second.setUseTexture(true);
	}
}

void Toolbar::setCursor(const std::string& iconName) {
	// Si on dessine on cache le curseur OS defaut
	if (availableCursorIcons.find(iconName) != availableCursorIcons.end()) {
		ofHideCursor();
		selectedCursor = availableCursorIcons[iconName];
	} else {
		ofShowCursor();
	}
}

void Toolbar::updateCursorIcon() {
	isDrawingActive = (rectangleToggle || circleToggle || lineToggle ||
					pointToggle || squareToggle || triangleToggle || freeformToggle ||
					sphereToggle || cubeToggle || cylinderToggle ||
					coneToggle || torusToggle || pyramidToggle || selectColourToggle);

	if (isDrawingActive) {
		//On cache le curseur par defaut de l'OS
		ofHideCursor();

		if (rectangleToggle) {
			setCursor("rectangle");
		} else if (circleToggle) {
			setCursor("circle");
		} else if (lineToggle) {
			setCursor("line");
		} else if (freeformToggle) {
			setCursor("freeform");
		} else if (pointToggle) {
			setCursor("point");
		} else if (squareToggle) {
			setCursor("square");
		} else if (triangleToggle) {
			setCursor("triangle");
		} else if (sphereToggle) {
			setCursor("sphere");
		} else if (cubeToggle) {
			setCursor("cube");
		} else if (cylinderToggle) {
			setCursor("cylinder");
		} else if (coneToggle) {
			setCursor("cone");
		} else if (torusToggle) {
			setCursor("torus");
		} else if (pyramidToggle) {
			setCursor("pyramid");
		} else {
			setCursor("default");
		}
	}
	else {
		ofShowCursor();
	}
}