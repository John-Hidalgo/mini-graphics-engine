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
	void variantNonePressed();
	void variantMetallicPressed();
	void variantPlasticPressed();
	void variantWireframePressed();
	void variantTransparentPressed();
	void draw();
	
	ofxPanel& getGui() { return dessinez; }
	bool isSelectingColor() const { return pickingColour; }
	
	ofxButton redBtn, greenBtn, blueBtn, yellowBtn;
	ofxToggle histogramToggle;
	ofxToggle skyBoxToggle;
	ofxToggle defRendToggle;
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
    ofImage selectedCursor;
	std::map<std::string, ofImage> availableCursorIcons;
private:
	Canvas* canvasRef;
	Model3D model3D;
	bool defRendering = false;
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
	ofxToggle sphereToggle;
	ofxToggle cubeToggle;
	ofxToggle cylinderToggle;
	ofxToggle coneToggle;
	ofxToggle torusToggle;
	ofxToggle pyramidToggle;

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
	void setExclusiveToggle(ShapeMode mode);

	void sphereToggleChanged(bool &val);
	void cubeToggleChanged(bool &val);
	void cylinderToggleChanged(bool &val);
	void coneToggleChanged(bool &val);
	void torusToggleChanged(bool &val);
	void pyramidToggleChanged(bool &val);
	void setExclusivePrimitiveToggle(Primitive3DType mode);

	void setCursor(const std::string& iconName);
	void updateCursorIcon();
};
