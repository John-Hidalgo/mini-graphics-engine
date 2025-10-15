#pragma once
#include "ofMain.h"
#include "ShapeDefs.h"

class ShapeRenderer {
public:
	static void drawPreview(ShapeMode currentMode,
						  const ofPoint& start,
						  const ofPoint& end,
						  const ofColor& currentColor,
						  const std::vector<ofPoint>& tempShapePoints,
						  bool drawing);
	static void drawShape(const Shape& s);
	
private:
	static void drawRectanglePreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawPointPreview(const ofPoint& start, const ofColor& color);
	static void drawSquarePreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawTrianglePreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawCirclePreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawLinePreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawFreeFormPreview(const std::vector<ofPoint>& points, const ofColor& color);
	static void drawHousePreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawTreePreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawTargetPreview(const ofPoint& start, const ofPoint& end, const ofColor& color);
	static void drawRectangle(const Shape& s);
	static void drawCircle(const Shape& s);
	static void drawPoint(const Shape& s);
	static void drawTriangle(const Shape& s);
	static void drawSquare(const Shape& s);
	static void drawLine(const Shape& s);
	static void drawFreeForm(const Shape& s);
	static void drawHouse(const Shape& s);
	static void drawTree(const Shape& s);
	static void drawTarget(const Shape& s);
};
