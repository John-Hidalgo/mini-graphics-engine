#pragma once
#include "ofMain.h"
#include <vector>

enum class ShapeMode { NONE, RECTANGLE, CIRCLE, LINE, FREEFORM, SQUARE, POINT, TRIANGLE, HOUSE,
	TREE, TARGET, CATMULL_ROM };

struct Shape {
	ShapeMode type;
	ofPoint start, end;
	ofColor contourColor;
	ofColor fillColor = ofColor(0,0,0,0);
	bool hasFill = false;
	float thickness = 2.0f;
	std::vector<ofPoint> points;
	ofPoint position{0, 0};
	float rotation{0.0f};
	float scale{1.0f};
	
	ofPoint getCenter() const {
			if (type == ShapeMode::POINT) return start + position;
			if (type == ShapeMode::FREEFORM && !points.empty()) {
				ofPoint center;
				for (auto& p : points) {
					center += p;
				}
				center /= points.size();
				return center + position;
			}
			return (start + end) * 0.5f + position;
		}
};
