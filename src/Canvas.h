#pragma once
#include "ofMain.h"
#include "ShapeDefs.h"
#include "Model3D.h"
#include <vector>
#include <map>

class Toolbar;

class Canvas {
public:
	Canvas();
	void setup(const ofRectangle& area,Toolbar* toolbar);
	void update();
	void draw();
	void drawPreview();
	
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void setCurrentMode(ShapeMode mode) { currentMode = mode; }
	ShapeMode getCurrentMode() const { return currentMode; }
	void setDrawingColor(ofColor color) { currentColor = color; }
	vector<Shape>& getShapes() {return shapes;}
	void setDrawingArea(const ofRectangle& area);
	void setHasImage(bool& value){hasModel = value;};
	
	ofParameter<ofColor> color_picker_background;
	ofParameter<ofColor> color_picker_ambient;
	ofParameter<ofColor> color_picker_diffuse;
	
	void undo();
	void clear();
	void loadImage(const std::string & path);
	void loadModel(const std::string& path); // load a 3D model
	void drawModel();
	void drawImage();
	
private:
	ofPoint start, end;
	bool drawing = false;
	ShapeMode currentMode = ShapeMode::NONE;
	vector<Shape> shapes;
	Shape tempShape;
	Toolbar* toolbarRef = nullptr;
	ofRectangle drawingArea;
	ofColor currentColor = ofColor(0, 0, 0);
	ofImage importedImage;
	Model3D model3D;// manages shaders, lighting, mesh, etc.
	bool hasModel = true; // only draw if true
	bool hasImage = false;

	void drawCanvas();
	void drawShape(const Shape& s);
	void drawCircle(const Shape& s);
	void drawFreeForm(const Shape& s);
	void drawRectangle(const Shape& s);
	
};
