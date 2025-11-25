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

	// 7.2 - Material Parameters pour les primitives
	ofxToggle primitives3DMaterialActive;
	ofParameter<ofColor> material_ambient_color_primitives3D;
	ofParameter<ofColor> material_diffuse_color_primitives3D;
	ofParameter<ofColor> material_emissive_color_primitives3D;
	ofParameter<ofColor> material_specular_color_primitives3D;
	ofxFloatSlider material_shininess_primitives3D;
	ofxFloatSlider material_metallic_primitives3D;
	ofxFloatSlider material_roughness_primitives3D;

	// 7.2 - Material Parameters pour les models
	ofxToggle modelMaterialActive;
	ofParameter<ofColor> material_ambient_color_model;
	ofParameter<ofColor> material_diffuse_color_model;
	ofParameter<ofColor> material_emissive_color_model;
	ofParameter<ofColor> material_specular_color_model;
	ofxFloatSlider material_shininess_model;

	// 8.2 - Pour les courbes parametriques (CatMull-Rom)
	//  Sliders individuels pour les points de controle
	ofxFloatSlider controlPointX0, controlPointX1, controlPointX2, controlPointX3, controlPointX4;
	ofxFloatSlider controlPointY0, controlPointY1, controlPointY2, controlPointY3, controlPointY4;
	ofxGuiGroup controlPointsGroup;

	// Méthodes pour gerer les points de controle
	void setupControlPointsSliders();
	void updateControlPointsSliders();
	void controlPointChanged(float& value);
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

    // 8.3 Pour les surfaces paramétriques
    // Contrôles pour la surface de Bézier
    ofxGuiGroup bezierSurfaceGroup;
    std::vector<std::vector<ofxFloatSlider*>> bezierControlPointSlidersX;
    std::vector<std::vector<ofxFloatSlider*>> bezierControlPointSlidersY;
    std::vector<std::vector<ofxFloatSlider*>> bezierControlPointSlidersZ;
    ofxIntSlider bezierResolutionSlider;

	// Slider pour les presets de surface de Bézier
	ofxIntSlider bezierPresetSlider;

	// Méthode pour gerer le changement de preset
	void bezierPresetChanged(int& preset);
	std::string getBezierPresetName(int preset);

	// Sous-groupes pour organiser les points de contrôle
	std::vector<ofxGuiGroup*> bezierControlPointGroups;

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

	// 7.2 - Pour les toggle de materials
	void primitives3DMaterialToggled(bool & val);
	void modelMaterialToggled(bool & val);

	// 8.2 - Pour les courbes parametriques (CatMull-Rom)
	void editCatmullRomControlPoints(int shapeIndex, const std::vector<ofPoint>& newPoints);
	void fixCatmullRomPoints(int shapeIndex);

    // 8.3 Pour les surfaces paramétriques
    void setupBezierSurfaceControls();
    void updateBezierSurfaceControls();
    void bezierControlPointChanged(float& value);
    void bezierResolutionChanged(int& value);
};
