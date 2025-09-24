#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ShapeDefs.h"
#include "Canvas.h"
#include "Model3D.h"

//class Canvas;

class Toolbar {
public:
	Toolbar();
	void setup(Canvas* canvas);
	void draw();
	
	ofxPanel& getGui() { return dessinez; }
	bool isSelectingColor() const { return pickingColour; }
	void setColorSlider(const ofColor& col){colorSlider = col;};
	
private:
	Canvas* canvasRef;
	Model3D model3D;
	
	ofxPanel dessinez;
	ofxToggle rectangleToggle;
	ofxToggle circleToggle;
	ofxToggle lineToggle;
	ofxToggle freeformToggle;
	ofxToggle selectColourToggle;
	ofxColorSlider colorSlider;
	ofxButton undoButton;
	
	ofxPanel importation;
	ofxButton importImageButton;
	ofxButton importModelButton;
	
	
	bool pickingColour = false;
	
	void rectangleToggleChanged(bool & val);
	void circleToggleChanged(bool & val);
	void lineToggleChanged(bool & val);
	void freeformToggleChanged(bool & val);
	void selectColourToggleChanged(bool& val);
	void colorChanged(ofColor& col);
	void importImagePressed();
	void undoButtonPressed();
	void importModelPressed();
	
	void setExclusiveToggle(ShapeMode mode);

};
