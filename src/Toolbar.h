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
	
	ofxButton redBtn, greenBtn, blueBtn, yellowBtn;
	void onRedPressed();
	void onGreenPressed();
	void onBluePressed();
	void onYellowPressed();
	
	ofColor currentColor;
	string currentColorName;
	ofxLabel currentColour;
	void setColorFromCanvas(ofColor& color,std::string& name);
	
private:
	Canvas* canvasRef;
	Model3D model3D;
	
	ofxPanel dessinez;
	ofxToggle rectangleToggle;
	ofxToggle circleToggle;
	ofxToggle lineToggle;
	ofxToggle pointToggle;
	ofxToggle triangleToggle;
	ofxToggle squareToggle;
	ofxToggle freeformToggle;
	ofxToggle selectColourToggle;
	ofxButton undoButton;
	
	ofxPanel importation;
	ofxButton importImageButton;
	ofxButton importModelButton;

	ofxPanel echantillonage;
	ofxButton echantillon1Button;
	ofxButton echantillon2Button;
	
	bool pickingColour = false;
	
	void rectangleToggleChanged(bool & val);
	void circleToggleChanged(bool & val);
	void lineToggleChanged(bool & val);
	void pointToggleChanged(bool & val);
	void triangleToggleChanged(bool & val);
	void squareToggleChanged(bool & val);
	void freeformToggleChanged(bool & val);
	void selectColourToggleChanged(bool& val);
	void colorChanged(ofColor& col);
	void importImagePressed();
	void undoButtonPressed();
	void importModelPressed();
	void echantillon1Pressed();
	void echantillon2Pressed();

	void setExclusiveToggle(ShapeMode mode);

};
