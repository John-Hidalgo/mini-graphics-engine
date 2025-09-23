#include "Toolbar.h"

Toolbar::Toolbar() : canvasRef(nullptr) {}

void Toolbar::setup(Canvas* canvas) {
	canvasRef = canvas;
	
	gui.setup("Tools");
	
	gui.add(rectangleToggle.setup("Rectangle", false));
	gui.add(circleToggle.setup("Cercle", false));
	gui.add(lineToggle.setup("Ligne", false));
	gui.add(freeformToggle.setup("Libre", false));
	gui.add(selectColourToggle.setup("coleur du canevas", false));
	gui.add(importImageButton.setup("Importez Image"));
	importImageButton.addListener(this, &Toolbar::importImagePressed);

	gui.add(colorSlider.setup("Couleur", ofColor(0, 0, 0), ofColor(0,0,0), ofColor(255,255,255)));
	static_cast<ofParameter<ofColor>&>(colorSlider.getParameter()).addListener(this, &Toolbar::colorChanged);

	
	gui.add(undoButton.setup("defaire"));

	rectangleToggle.addListener(this, &Toolbar::rectangleToggleChanged);
	circleToggle.addListener(this, &Toolbar::circleToggleChanged);
	lineToggle.addListener(this, &Toolbar::lineToggleChanged);
	freeformToggle.addListener(this, &Toolbar::freeformToggleChanged);
	selectColourToggle.addListener(this, &Toolbar::selectColourToggleChanged);
	undoButton.addListener(this, &Toolbar::undoButtonPressed);
}

void Toolbar::draw() {
	gui.draw();
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


