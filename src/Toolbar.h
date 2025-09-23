#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ShapeDefs.h"
#include "Canvas.h"

//class Canvas;

class Toolbar {
public:
	Toolbar();
	void setup(Canvas* canvas);
	void draw();
	
	ofxPanel& getGui() { return gui; }
	bool isSelectingColor() const { return pickingColour; }
	void setColorSlider(const ofColor& col){colorSlider = col;};
	
private:
	ofxPanel gui;
	Canvas* canvasRef;
	bool pickingColour = false;
	
	ofxToggle rectangleToggle;
	ofxToggle circleToggle;
	ofxToggle lineToggle;
	ofxToggle freeformToggle;
	ofxToggle selectColourToggle;
	ofxColorSlider colorSlider;
	ofxButton importImageButton;
	ofxButton undoButton;
	
	void rectangleToggleChanged(bool & val);
	void circleToggleChanged(bool & val);
	void lineToggleChanged(bool & val);
	void freeformToggleChanged(bool & val);
	void selectColourToggleChanged(bool& val);
	void colorChanged(ofColor& col);
	void importImagePressed();
	void undoButtonPressed();
	
	void setExclusiveToggle(ShapeMode mode);

};
