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
	void selectShapesInArea(const ofRectangle& selectionRect);
	void clearSelection();
	ofxFloatSlider scaleSlider;
	ofxFloatSlider positionSlider;
	ofxPanel modelEditorPanel;
	
	ofParameter<ofColor> color_picker_background;
	ofParameter<ofColor> color_picker_ambient;
	ofParameter<ofColor> color_picker_diffuse;
	std::vector<int> selectedShapeIndices;
	std::vector<int> selectedModelIndices;
	std::vector<int> selectedPrimitiveIndices;
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
	ofxButton deleteButton3DModel;
	ofxFloatSlider hueSlider;
	ofxFloatSlider satSlider;
	ofxFloatSlider briSlider;
	ofxToggle contourToggle;
	ofxToggle fillToggle;
	ofPoint selectionStart;
	ofRectangle selectionRect;
	ofxToggle backgroundToggle;
	

	void deleteButtonPressed();
	void deleteButton3DModelPressed();
	void thicknessChanged(float & val);
	void colorComponentChanged(int & value);
	void drawShapeList();
	//void adjustShapeColor();
	void colorChanged(ofColor& col);
	void hsbChanged(float & val);
	void contourToggled(bool & val);
	void fillToggled(bool & val);
	
	void drawModelList();
	void drawPrimitivesList();
	void backgroundColorChanged(ofColor& color);
	void backgroundToggled(bool& val);
	void ensureSingleToggle();
};
