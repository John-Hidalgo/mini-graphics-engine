#pragma once
#include "ofMain.h"
#include <vector>

enum class ShapeMode { NONE, RECTANGLE, CIRCLE, LINE, FREEFORM, SQUARE, POINT, TRIANGLE, HOUSE,
	TREE, TARGET };

struct Shape {
	ShapeMode type;
	ofPoint start, end;
	ofColor contourColor;
	ofColor fillColor = ofColor(0,0,0,0);
	bool hasFill = false;
	float thickness = 2.0f;
	std::vector<ofPoint> points;
};
