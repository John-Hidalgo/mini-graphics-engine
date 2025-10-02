#pragma once
#include "ofMain.h"
#include "ShapeDefs.h"
#include "Model3D.h"
#include <vector>
#include <map>


class SceneGraph;
class Toolbar;

class Canvas {
public:
	Canvas();
	void setup(const ofRectangle& area,Toolbar* toolbar,SceneGraph* sceneGraph);
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
	
	std::vector<std::unique_ptr<Model3D>> models;
	vector<Shape> shapes;
	ofParameter<ofColor> color_picker_background;
	ofParameter<ofColor> color_picker_ambient;
	ofParameter<ofColor> color_picker_diffuse;
	
	void undo();
	void clear();
	void loadImage(const std::string & path);
	void loadModel(const std::string& path);
	void drawModel();
	void drawImage();
	void calculateModelsPosition();
	
	void draw2d();
	void draw3d();
	
	
	
private:
	ofPoint start, end;
	bool drawing = false;
	ShapeMode currentMode = ShapeMode::NONE;
	Shape tempShape;
	Toolbar* toolbarRef = nullptr;
	ofRectangle drawingArea;
	ofColor currentColor = ofColor(0, 0, 0);
	ofImage importedImage;
	Model3D model3D;
	bool hasModel = true;
	bool hasImage = false;
	SceneGraph* sceneGraphRef = nullptr;

	void drawCanvas();
	void drawShape(const Shape& s);
	void drawCircle(const Shape& s);
	void drawFreeForm(const Shape& s);
	void drawRectangle(const Shape& s);
	
};
