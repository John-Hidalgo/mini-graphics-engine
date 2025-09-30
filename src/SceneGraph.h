#pragma once
#include "ofMain.h"
#include "Canvas.h"
#include "ofxGui.h"

class SceneGraph {
public:
	SceneGraph();
	void setup(Canvas* canvas, const ofRectangle& area);
	void draw();
	void mousePressed(int x, int y, int button);
	void setPanelArea(const ofRectangle& area);
	ofxFloatSlider scaleSlider;
	ofxFloatSlider positionSlider;
	ofxPanel modelEditorPanel;
	
	ofParameter<ofColor> color_picker_background;
	ofParameter<ofColor> color_picker_ambient;
	ofParameter<ofColor> color_picker_diffuse;
	std::vector<int> selectedShapeIndices;
	std::vector<int> selectedModelIndices;
	ofParameter<ofColor> colour2dShapes;
	
private:
	Canvas* canvasRef;
	ofRectangle panelArea;

	float x, y, width, height;
	float panelPadding = 10;
	int selectedIndex = -1;
	
	ofxPanel gui;
	ofxFloatSlider thicknessSlider;
	//ofxColorSlider colorSlider;
	ofxButton deleteButton;
	ofxFloatSlider hueSlider;
	ofxFloatSlider satSlider;
	ofxFloatSlider briSlider;
	ofxToggle contourToggle;
	ofxToggle fillToggle;
	

	void deleteButtonPressed();
	void thicknessChanged(float & val);
	void colorComponentChanged(int & value);
	void drawShapeList();
	//void adjustShapeColor();
	void colorChanged(ofColor& col);
	void hsbChanged(float & val);
	void contourToggled(bool & val);
	void fillToggled(bool & val);
	
	void drawModelList();
};
