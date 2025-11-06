#pragma once
#include "ofMain.h"
#include "Canvas.h"
#include "ofxGui.h"

class SceneGraph {
public:
	SceneGraph();
	void setup(Canvas* canvas, const ofRectangle& area);
	void setupModelPanel();
	void setupNormalMappingVisible(bool visible);
	void draw();
	void mousePressed(int x, int y, int button);
	void setPanelArea(const ofRectangle& area);
	//void setEnableNormalMapping(bool& val){enableNormalMapping = val;};
	void selectShapesInArea(const ofRectangle& selectionRect);
	void select3DObjectsInArea(const ofRectangle& selectionRect);
	void clearSelection();
	ofxFloatSlider scaleSlider;
	ofxFloatSlider positionSlider;
	ofxPanel modelEditorPanel;
	
	ofParameter<ofColor> color_picker_background;
	ofParameter<ofColor> color_picker_ambient;
	ofParameter<ofColor> color_picker_diffuse;
	ofParameter<ofColor> color_picker_background_primitives3D;
	ofParameter<ofColor> color_picker_ambient_primitives3D;
	ofParameter<ofColor> color_picker_diffuse_primitives3D;
	ofxButton textureInversionButton;
	ofxButton textureWeierstrassButton;
	ofxButton textureNormalMappingButton;
	ofxGuiGroup textureGroup;
	ofxToggle animationSurfaceToggle;
	ofxToggle animationColourToggle;
	ofxGuiGroup animationGroup;
	ofxButton HDRDayToggle;
	ofxButton HDRNightToggle;
	ofxGuiGroup HDRGroup;
	std::vector<int> selectedShapeIndices;
	std::vector<int> selectedModelIndices;
	std::vector<int> selectedPrimitiveIndices;
	ofParameter<ofColor> colour2dShapes;

	ofxPanel primitives3DEditorPanel;
	ofxFloatSlider primitives3DSizeSlider;
	ofxIntSlider primitives3DPosXSlider;
	ofxIntSlider primitives3DPosYSlider;
	ofxIntSlider primitives3DPosZSlider;
	
	ofxFloatSlider translateShapeXSlider;
	ofxFloatSlider translateShapeYSlider;
	ofxButton rotateShapeRightButton;
	ofxButton rotateShapeLeftButton;
	ofxFloatSlider scaleShapeSlider;

	std::vector<int> selectedLightIndices;
	ofxPanel lightEditorPanel;
	ofxFloatSlider lightPosXSlider;
	ofxFloatSlider lightPosYSlider;
	ofxFloatSlider lightPosZSlider;
	ofParameter<ofColor> lightColorPicker;
	ofxFloatSlider lightAttenuationSlider;
	ofxButton deleteLightButton;
private:
	Canvas* canvasRef;
	ofRectangle panelArea;

	float x, y, width, height;
	float panelPadding = 10;
	int selectedIndex = -1;
	int listsStartHeight = 550;
	bool enableNormalMapping = false;
	
	ofxPanel gui;
	ofxFloatSlider thicknessSlider;
	//ofxColorSlider colorSlider;
	ofxButton deleteButton;
	ofxButton deleteButton3DModel;
	ofxButton deleteButtonPrimitives3D;
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
	void deleteButtonPrimitives3DPressed();
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
	
	void translationShapeChanged(float& val);
	void rotateShapeRightPressed();
	void rotateShapeLeftPressed();
	void scaleShapeChanged(float& val);
	void textureInversionPressed();
	void textureWeierstrassPressed();
	void textureNormalMappingPressed();
	void animateSurfacePressed(bool& val);
	void animateColourPressed(bool& val);
	void HDRDayPressed();
	void HDRNightPressed();

	void drawLightList();
	void deleteButtonLightPressed();
	void lightPositionChanged(float &val);
	void lightAttenuationChanged(float& val);
	void lightColorChanged(ofColor& col);
};
