#include "ShapeRenderer.h"

void ShapeRenderer::drawPreview(ShapeMode currentMode,
							   const ofPoint& start,
							   const ofPoint& end,
							   const ofColor& currentColor,
							   const std::vector<ofPoint>& tempShapePoints,
							   bool drawing) {
	if (!drawing) return;
	
	ofSetColor(currentColor);
	ofNoFill();

	switch (currentMode) {
		case ShapeMode::RECTANGLE:
			drawRectanglePreview(start, end, currentColor);
			break;
		case ShapeMode::POINT:
			drawPointPreview(start, currentColor);
			break;
		case ShapeMode::SQUARE:
			drawSquarePreview(start, end, currentColor);
			break;
		case ShapeMode::TRIANGLE:
			drawTrianglePreview(start, end, currentColor);
			break;
		case ShapeMode::CIRCLE:
			drawCirclePreview(start, end, currentColor);
			break;
		case ShapeMode::LINE:
			drawLinePreview(start, end, currentColor);
			break;
		case ShapeMode::FREEFORM:
			drawFreeFormPreview(tempShapePoints, currentColor);
			break;
		case ShapeMode::HOUSE:
			drawHousePreview(start, end, currentColor);
			break;
		case ShapeMode::TREE:
			drawTreePreview(start, end, currentColor);
			break;
		case ShapeMode::TARGET:
			drawTargetPreview(start, end, currentColor);
			break;
		default:
			break;
	}
}

void ShapeRenderer::drawShape(const Shape& s) {
	switch (s.type) {
		case ShapeMode::RECTANGLE: drawRectangle(s); break;
		case ShapeMode::CIRCLE: drawCircle(s); break;
		case ShapeMode::LINE: drawLine(s); break;
		case ShapeMode::FREEFORM: drawFreeForm(s); break;
		case ShapeMode::POINT: drawPoint(s); break;
		case ShapeMode::TRIANGLE: drawTriangle(s); break;
		case ShapeMode::SQUARE: drawSquare(s); break;
		case ShapeMode::HOUSE: drawHouse(s); break;
		case ShapeMode::TREE: drawTree(s); break;
		case ShapeMode::TARGET: drawTarget(s); break;
		default: break;
	}
}

// COPY ALL YOUR EXISTING DRAWING METHODS EXACTLY AS THEY ARE
// Just replace "Canvas::" with "ShapeRenderer::"

void ShapeRenderer::drawRectanglePreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	float x = std::min(start.x, end.x);
	float y = std::min(start.y, end.y);
	float w = std::abs(end.x - start.x);
	float h = std::abs(end.y - start.y);
	ofDrawRectangle(x, y, w, h);
}

void ShapeRenderer::drawPointPreview(const ofPoint& start, const ofColor& color) {
	float pointSize = 5.0f;
	ofDrawCircle(start.x, start.y, pointSize);
}

void ShapeRenderer::drawSquarePreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	float x = std::min(start.x, end.x);
	float y = std::min(start.y, end.y);
	float w = std::abs(end.x - start.x);
	float h = std::abs(end.y - start.y);
	float size = std::min(w, h);
	if (end.x < start.x) x = start.x - size;
	if (end.y < start.y) y = start.y - size;
	ofDrawRectangle(x, y, size, size);
}

void ShapeRenderer::drawTrianglePreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	ofPoint p1 = start;
	ofPoint p2 = ofPoint(end.x, start.y);
	ofPoint p3 = ofPoint((start.x + end.x) / 2, end.y);
	ofDrawTriangle(p1, p2, p3);
}

void ShapeRenderer::drawCirclePreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	float radius = start.distance(end);
	ofDrawCircle(start, radius);
}

void ShapeRenderer::drawLinePreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	ofDrawLine(start, end);
}

void ShapeRenderer::drawFreeFormPreview(const std::vector<ofPoint>& points, const ofColor& color) {
	ofBeginShape();
	for (auto &p : points) {
		ofVertex(p.x, p.y);
	}
	ofEndShape(false);
}

void ShapeRenderer::drawHousePreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	float x = std::min(start.x, end.x);
	float y = std::min(start.y, end.y);
	float w = std::abs(end.x - start.x);
	float h = std::abs(end.y - start.y);

	ofDrawTriangle(
		ofPoint(x, y + h * 0.3),
		ofPoint(x + w / 2, y),
		ofPoint(x + w, y + h * 0.3)
	);
	ofDrawRectangle(x, y + h * 0.3, w, h * 0.7);
}

void ShapeRenderer::drawTreePreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	float x = std::min(start.x, end.x);
	float y = std::min(start.y, end.y);
	float w = std::abs(end.x - start.x);
	float h = std::abs(end.y - start.y);

	ofDrawCircle(x + w / 2, y + h * 0.4, w * 0.4);
	ofDrawRectangle(x + w * 0.4, y + h * 0.6, w * 0.2, h * 0.4);
}

void ShapeRenderer::drawTargetPreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
	float radius = start.distance(end);
	for (int i = 0; i < 5; ++i) {
		float r = radius * (1.0 - i * 0.2);
		ofDrawCircle(start, r);
	}
}

// COPY ALL YOUR EXISTING SHAPE DRAWING METHODS HERE EXACTLY AS THEY ARE
// Just change "Canvas::drawRectangle" to "ShapeRenderer::drawRectangle", etc.


// Continue copying ALL your other draw methods exactly as they are...
// drawCircle, drawPoint, drawTriangle, drawSquare, drawLine, drawFreeForm,
// drawHouse, drawTree, drawTarget - ALL OF THEM
void ShapeRenderer::drawRectangle(const Shape& s) {
	ofPushMatrix();
	float x = std::min(s.start.x, s.end.x);
	float y = std::min(s.start.y, s.end.y);
	float w = std::abs(s.end.x - s.start.x);
	float h = std::abs(s.end.y - s.start.y);
	ofPoint localCenter(x + w/2, y + h/2);
	ofTranslate(s.position);
	ofTranslate(localCenter);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-localCenter);
	
	ofFill();
	ofSetColor(s.fillColor);
	ofDrawRectangle(x, y, w, h);

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawRectangle(x, y, w, h);
	
	ofPopMatrix();
}
void ShapeRenderer::drawCircle(const Shape& s) {
	ofPushMatrix();
	ofPoint center = s.start;
	float radius = s.start.distance(s.end);
	ofTranslate(s.position);
	ofTranslate(center);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-center);
	ofFill();
	ofSetColor(s.fillColor);
	ofDrawCircle(0, 0, radius);

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawCircle(0, 0, radius);
	
	ofPopMatrix();
}
void ShapeRenderer::drawPoint(const Shape& s) {
	ofPushMatrix();
	ofPoint center = s.start + s.position;
	ofTranslate(center);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-center);
	float pointSize = 5.0f;
	ofFill();
	ofSetColor(s.fillColor);
	ofDrawCircle(s.start, pointSize);
	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawCircle(s.start, pointSize);
	ofPopMatrix();
}
void ShapeRenderer::drawTriangle(const Shape& s) {
	ofPushMatrix();
	ofPoint center = s.getCenter();
	ofTranslate(center);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-center);
	ofPoint p1 = s.start + s.position;
	ofPoint p2 = ofPoint(s.end.x, s.start.y) + s.position;
	ofPoint p3 = ofPoint((s.start.x + s.end.x) / 2, s.end.y) + s.position;
	ofFill();
	ofSetColor(s.fillColor);
	ofDrawTriangle(p1, p2, p3);
	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawTriangle(p1, p2, p3);
	ofPopMatrix();
}
void ShapeRenderer::drawSquare(const Shape& s) {
	ofPushMatrix();
	
	// Apply transformations
	ofPoint center = s.getCenter();
	ofTranslate(center);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-center);
	
	float w = std::abs(s.end.x - s.start.x);
	float h = std::abs(s.end.y - s.start.y);
	float size = std::min(w, h);
	float x = s.start.x + s.position.x;
	float y = s.start.y + s.position.y;
	
	if (s.end.x < s.start.x) x = s.start.x - size + s.position.x;
	if (s.end.y < s.start.y) y = s.start.y - size + s.position.y;
	
	ofFill();
	ofSetColor(s.fillColor);
	ofDrawRectangle(x, y, size, size);
	
	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawRectangle(x, y, size, size);
	
	ofPopMatrix();
}
void ShapeRenderer::drawLine(const Shape& s) {
	ofPushMatrix();
	ofPoint center = s.getCenter();
	ofTranslate(center);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-center);
	ofPoint start = s.start + s.position;
	ofPoint end = s.end + s.position;
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawLine(start, end);
	ofPopMatrix();
}
void ShapeRenderer::drawFreeForm(const Shape& s) {
	ofPushMatrix();
	ofPoint center = s.getCenter();
	ofTranslate(center);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-center);
	if (!s.points.empty()) {
		ofFill();
		ofSetColor(s.fillColor);
		ofBeginShape();
		for (auto& p : s.points) {
			ofVertex(p.x + s.position.x, p.y + s.position.y);
		}
		ofEndShape(true);
	}
	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofBeginShape();
	for (auto& p : s.points) {
		ofVertex(p.x + s.position.x, p.y + s.position.y);
	}
	ofEndShape(false);
	ofPopMatrix();
}

void ShapeRenderer::drawHouse(const Shape& s) {
	float x = std::min(s.start.x, s.end.x);
	float y = std::min(s.start.y, s.end.y);
	float w = std::abs(s.end.x - s.start.x);
	float h = std::abs(s.end.y - s.start.y);

	ofFill();
	ofSetColor(s.fillColor);
	ofDrawRectangle(x, y + h * 0.3, w, h * 0.7);

	ofNoFill();
	ofSetColor(s.contourColor);
	ofSetLineWidth(s.thickness);
	ofDrawRectangle(x, y + h * 0.3, w, h * 0.7);

	ofFill();
	ofSetColor(s.fillColor);
	ofDrawTriangle(
		ofPoint(x, y + h * 0.3),
		ofPoint(x + w / 2, y),
		ofPoint(x + w, y + h * 0.3)
	);

	ofNoFill();
	ofSetColor(s.contourColor);
	ofSetLineWidth(s.thickness);
	ofDrawTriangle(
		ofPoint(x, y + h * 0.3),
		ofPoint(x + w / 2, y),
		ofPoint(x + w, y + h * 0.3)
	);
}

void ShapeRenderer::drawTree(const Shape& s) {
	float x = std::min(s.start.x, s.end.x);
	float y = std::min(s.start.y, s.end.y);
	float w = std::abs(s.end.x - s.start.x);
	float h = std::abs(s.end.y - s.start.y);

	ofFill();
	ofSetColor(139, 69, 19); // brown
	ofDrawRectangle(x + w * 0.4, y + h * 0.6, w * 0.2, h * 0.4);

	ofFill();
	ofSetColor(34, 139, 34); // green
	ofDrawCircle(x + w / 2, y + h * 0.4, w * 0.4);

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawRectangle(x + w * 0.4, y + h * 0.6, w * 0.2, h * 0.4);
	ofDrawCircle(x + w / 2, y + h * 0.4, w * 0.4);
}

void ShapeRenderer::drawTarget(const Shape& s) {
	float radius = s.start.distance(s.end);

	for (int i = 0; i < 5; ++i) {
		float r = radius * (1.0 - i * 0.2);
		if (i % 2 == 0)
			ofSetColor(s.fillColor);
		else
			ofSetColor(s.contourColor);

		ofFill();
		ofDrawCircle(s.start, r);
	}

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(0);
	ofDrawCircle(s.start, radius);
}



