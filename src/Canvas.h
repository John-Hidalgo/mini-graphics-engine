#pragma once
#include "ofMain.h"
#include "ShapeDefs.h"
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
	
	void undo();
	void clear();
	void loadImage(const std::string & path);
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
	bool hasImage = false;

	void drawShape(const Shape& s);
	void drawCircle(const Shape& s);
	void drawFreeForm(const Shape& s);
	void drawRectangle(const Shape& s);
	
};
