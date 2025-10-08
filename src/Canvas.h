#pragma once
#include "ofMain.h"
#include "ShapeDefs.h"
#include "Model3D.h"
#include "Primitives3DDefs.h"
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
    vector<std::unique_ptr<Model3D>>& getModels() {return models;}
    vector<Primitive3D>& getPrimitives3D() {return primitives3D;}

    void setDrawingArea(const ofRectangle& area);
	void setHasImage(bool& value){hasModel = value;};
	
	vector<std::unique_ptr<Model3D>> models;
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

	void setCurrentPrimitiveMode(Primitive3DType mode);
	Primitive3DType getCurrentPrimitiveMode() const { return currentPrimitiveMode; }

	ofCamera* activeCamera = nullptr;
	ofRectangle cameraViewport;
	void setActiveCamera(ofCamera* cam, const ofRectangle& viewport);
	ofPoint customScreenToWorld(int x, int y, float planeZ = 0.0f);
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
	float zoomLevel = 1.0f;
	float minZoom = 0.1f;
	float maxZoom = 5.0f;

	void drawCanvas();
	void drawShape(const Shape& s);
	void drawCircle(const Shape& s);
	void drawFreeForm(const Shape& s);
	void drawRectangle(const Shape& s);
	void drawPoint(const Shape& s);
	void drawTriangle(const Shape& s);
	void drawSquare(const Shape& s);

	std::vector<Primitive3D> primitives3D;
	Primitive3DType currentPrimitiveMode = Primitive3DType::NONE;
	bool placingPrimitive = false;
	bool drawingPrimitive = false;
	ofPoint primitiveStartPos;

	Primitive3D tempPrimitive;

	void addPrimitive3D(Primitive3DType type, const ofPoint& position, float size);

	void drawPrimitives3D();

	void drawPrimitivePreview();

};
