#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ShapeDefs.h"
#include "Canvas.h"
#include "Model3D.h"
#include "Material.h"

//class Canvas;

enum class MaterialType {
	PBRMetal,
	PBRPlastic,
	PBRRough,
	MetalPolished,
	MetalBrushed,
	RoughSurface,
	SmoothSurface,
};



class Toolbar {
public:
	Toolbar();
	void setup(Canvas* canvas);
	void variantNonePressed();
	void variantMetallicPressed();
	void variantPlasticPressed();
	void variantWireframePressed();
	void variantTransparentPressed();
	void draw();
	MaterialType selectedMaterial = MaterialType::PBRMetal;
	
	ofxPanel& getGui() { return dessinez; }
	bool isSelectingColor() const { return pickingColour; }
	
	ofxButton redBtn, greenBtn, blueBtn, yellowBtn;
	ofxToggle histogramToggle;
	ofxToggle skyBoxToggle;
	ofxToggle defRendToggle;
	ofxToggle rayRenderingToggle;
	ofxToggle rayRenderingGIToggle;
	ofxGuiGroup variantGroup;
	ofxButton variantNoneButton;
	ofxButton variantMetallicButton;
	ofxButton variantPlasticButton;
	ofxButton variantWireframeButton;
	ofxButton variantTransparentButton;
	Lighting lighting = Lighting::LAMBERT;
	ModelVariant selectedVariant = ModelVariant::None;
	void onRedPressed();
	void onGreenPressed();
	void onBluePressed();
	void onYellowPressed();

	void setSelectedVariant(ModelVariant variant);
	void updateVariantButtonColors();
	
	ofColor currentColor;
	string currentColorName;
	ofxLabel currentColour;
	void setColorFromCanvas(ofColor& color,std::string& name);

	void loadCursorIcons();
	bool isDrawingActive = false;
	bool getDefRendering(){return defRendering;};
	bool getRayRendering(){return rayRendering;};
	bool getRayRenderingGI(){return rayRenderingGI;};
    ofImage selectedCursor;
	std::map<std::string, ofImage> availableCursorIcons;
private:
	Canvas* canvasRef;
	Model3D model3D;
	bool defRendering = false;
	bool rayRendering = false;
	bool rayRenderingGI = false;
	ofxPanel dessinez;
	ofxToggle rectangleToggle;
	ofxToggle circleToggle;
	ofxToggle lineToggle;
	ofxToggle pointToggle;
	ofxToggle triangleToggle;
	ofxToggle squareToggle;
	ofxToggle freeformToggle;
	ofxToggle houseToggle;
	ofxToggle treeToggle;
	ofxToggle targetToggle;

    // 8.2 - Pour les courbes parametriques (CatMull-Rom)
	ofxToggle catmullRomToggle;

	ofxToggle selectColourToggle;
	ofxButton undoButton;
	
	ofxPanel importation;
	ofxButton importImageButton;
	ofxButton importModelButton;

	ofxPanel echantillonage;
	ofxButton echantillonageButton;
	ofxToggle echantillonageUniqueToggle;
	ofxToggle echantillonageMultipleToggle;
	ofxIntSlider gridColumnsSlider;
	ofxIntSlider gridRowsSlider;
	ofxFloatSlider imageDisplayScaleSlider;
	ofxToggle maintainAspectRatioToggle;

	ofxIntSlider sampleXSlider;
	ofxIntSlider sampleYSlider;
	ofxIntSlider sampleWidthSlider;
	ofxIntSlider sampleHeightSlider;


	ofxPanel primitives3DGroup;
	ofxGuiGroup materialGroup;
	ofxToggle sphereToggle;
	ofxToggle cubeToggle;
	ofxToggle cylinderToggle;
	ofxToggle coneToggle;
	ofxToggle torusToggle;
	ofxToggle pyramidToggle;
	ofxButton materialPBRMetal;
	ofxButton materialPBRPlastic;
	ofxButton materialPBRRough;
	ofxButton materialMetalPolished;
	ofxButton materialMetalBrushed;
	ofxButton materialRoughSurface;
	ofxButton materialSmoothSurface;
	Material currentMaterial = Material(
		ofColor(40, 40, 40),        // Ambient
		ofColor(200, 200, 200),     // Diffuse (albedo)
		ofColor(0, 0, 0),           // Emissive
		ofColor(255, 255, 255),     // Specular
		64.0f,                      // Shininess
		1.0f,                       // Metallic
		0.3f                        // Roughness
	);;

    // 8.3 Pour les surfaces paramétriques
    ofxToggle bezierSurfaceToggle;

	ofxPanel lightGroup;
	ofxButton addAmbientLightBtn;
	ofxButton addDirectionalLightBtn;
	ofxButton addPointLightBtn;
	ofxButton addSpotLightBtn;



	bool pickingColour = false;

	void onAddAmbientLightPressed();
	void onAddDirectionalLightPressed();
	void onAddPointLightPressed();
	void onAddSpotLightPressed();
	void rectangleToggleChanged(bool & val);
	void circleToggleChanged(bool & val);
	void lineToggleChanged(bool & val);
	void pointToggleChanged(bool & val);
	void triangleToggleChanged(bool & val);
	void squareToggleChanged(bool & val);
	void freeformToggleChanged(bool & val);
	void houseToggleChanged(bool & val);
	void treeToggleChanged(bool & val);
	void targetToggleChanged(bool & val);

    // 8.2 - Pour les courbes parametriques (CatMull-Rom)
    void catMullRomToggleChanged(bool & val);

	void selectColourToggleChanged(bool& val);
	void colorChanged(ofColor& col);
	void importImagePressed();
	void undoButtonPressed();
	void importModelPressed();
	void echantillonagePressed();
	void echantillonageUniquePressed();
	void echantillonageMultiplePressed();
	void processEchantillonMultiple(const std::string& folderPath);
	void imageDisplayScaleChanged(float &val);
	void maintainAspectRatioToggled(bool &val);
	void histogramToggleChanged(bool& val);
	void skyBoxToggleChanged(bool& val);
	void defRenderingToggled(bool& val);
	void rayRenderingToggled(bool& val);
	void rayRenderingGIToggled(bool& val);
	void setExclusiveToggle(ShapeMode mode);

	void sphereToggleChanged(bool &val);
	void cubeToggleChanged(bool &val);
	void cylinderToggleChanged(bool &val);
	void coneToggleChanged(bool &val);
	void torusToggleChanged(bool &val);
	void pyramidToggleChanged(bool &val);

    // 8.3 Pour les surfaces paramétriques
    void bezierSurfaceToggleChanged(bool &val);

	void setExclusivePrimitiveToggle(Primitive3DType mode);

	void setCursor(const std::string& iconName);
	void updateCursorIcon();

	void materialPBRMetalPressed();
	void materialPBRPlasticPressed();
	void materialPBRRoughPressed();

	void materialMetalPolishedPressed();
	void materialMetalBrushedPressed();

	void materialRoughSurfacePressed();
	void materialSmoothSurfacePressed();
	void setSelectedMaterial(MaterialType mat);
	void updateMaterialButtonColors();
};
