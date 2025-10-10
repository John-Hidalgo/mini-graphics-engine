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
	ofxGuiGroup variantGroup;
	ofxButton variantNoneButton;
	ofxButton variantMetallicButton;
	ofxButton variantPlasticButton;
	ofxButton variantWireframeButton;
	ofxButton variantTransparentButton;

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
    ofImage selectedCursor;
	std::map<std::string, ofImage> availableCursorIcons;
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
	ofxButton echantillonageButton;
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
	void echantillonagePressed();
	void histogramToggleChanged(bool& val);

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
