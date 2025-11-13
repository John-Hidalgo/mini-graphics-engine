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
        case ShapeMode::CATMULL_ROM:
            drawCatmullRomPreview(start, end, currentColor);
            break;
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
        case ShapeMode::CATMULL_ROM: drawCatmullRom(s); break;
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


// 8.2 - Pour les courbes parametriques

// Interpolation pour Catmull-Rom, liens:
// https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
// https://qroph.github.io/2018/07/30/smooth-paths-using-catmull-rom-splines.html
ofPoint ShapeRenderer::catmullRomInterpolate(const ofPoint& p0, const ofPoint& p1,
                                             const ofPoint& p2, const ofPoint& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    // Formule de Catmull-Rom
    ofPoint resultat;
	resultat.x = 0.5f * ((2.0f * p1.x) +
                       (-p0.x + p2.x) * t +
                       (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2 +
                       (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3);

	resultat.y = 0.5f * ((2.0f * p1.y) +
                       (-p0.y + p2.y) * t +
                       (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2 +
                       (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3);
    return resultat;
}

// Génere des points de la courbe Catmull-Rom
std::vector<ofPoint> ShapeRenderer::generateCatmullRomCurve(const std::vector<ofPoint>& controlPoints,
                                                            int segments) {
    std::vector<ofPoint> curvePoints;

    if (controlPoints.size() < 4) {
        // Si pas assez de points pour Catmull-Rom, on retourne les points de controle
        return controlPoints;
    }

    // Generer notre courbe entre les points de controle internes
    for (size_t i = 1; i < controlPoints.size() - 2; i++) {
        const ofPoint& p0 = controlPoints[i - 1];
        const ofPoint& p1 = controlPoints[i];
        const ofPoint& p2 = controlPoints[i + 1];
        const ofPoint& p3 = controlPoints[i + 2];

        for (int j = 0; j <= segments; j++) {
            float t = (float)j / (float)segments;
            ofPoint interpolated = catmullRomInterpolate(p0, p1, p2, p3, t);
            curvePoints.push_back(interpolated);
        }
    }

    return curvePoints;
}

// Preview comme pour les shapes / primitives
void ShapeRenderer::drawCatmullRomPreview(const ofPoint& start, const ofPoint& end, const ofColor& color) {
    // Pour le preview, utiliser les points de controle calculés
    std::vector<ofPoint> controlPoints;

    float width = std::abs(end.x - start.x);
    float height = std::abs(end.y - start.y);

    // Points de controle répartis
    controlPoints.push_back(ofPoint(start.x, start.y));
    controlPoints.push_back(ofPoint(start.x + width * 0.25f, start.y - height * 0.3f));
    controlPoints.push_back(ofPoint(start.x + width * 0.5f, start.y + height * 0.2f));
    controlPoints.push_back(ofPoint(start.x + width * 0.75f, start.y - height * 0.1f));
    controlPoints.push_back(ofPoint(end.x, end.y));

    // Generer et dessiner la courbe
    std::vector<ofPoint> curvePoints = generateCatmullRomCurve(controlPoints);

    if (!curvePoints.empty()) {
        ofBeginShape();
        for (const auto& point : curvePoints) {
            ofVertex(point.x, point.y);
        }
        ofEndShape(false);
    }

    // Dessiner les points de controle pour la previsualisation
    ofFill();
	// Petite transparence
    ofSetColor(color, 150);
    for (const auto& point : controlPoints) {
		// On dessines les points de controle
        ofDrawCircle(point, 4.0f);
    }

    // Dessiner les lignes entre les points de controle
    ofSetColor(color, 100);
    ofSetLineWidth(1);
    for (size_t i = 0; i < controlPoints.size() - 1; i++) {
        ofDrawLine(controlPoints[i], controlPoints[i + 1]);
    }

    ofNoFill();
}

// Forme finale de la courbe Catmull-Rom
void ShapeRenderer::drawCatmullRom(const Shape& s) {
	ofPushMatrix();

	// Appliquer les transformations globales
	ofPoint center = s.getCenter();
	ofTranslate(s.position);
	ofTranslate(center);
	ofRotateDeg(s.rotation);
	ofScale(s.scale, s.scale);
	ofTranslate(-center);

	// Générer la courbe a partir des points ABSOLUS
	std::vector<ofPoint> curvePoints = generateCatmullRomCurve(s.points);

	// Dessiner la courbe filled au besoin
	if (s.hasFill && curvePoints.size() > 2) {
		ofFill();
		ofSetColor(s.fillColor);
		ofBeginShape();
		for (const auto& point : curvePoints) {
			ofVertex(point.x, point.y);
		}
		ofEndShape(true);
	}

	// Dessiner le contour
	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);

	if (!curvePoints.empty()) {
		ofBeginShape();
		for (const auto& point : curvePoints) {
			ofVertex(point.x, point.y);
		}
		ofEndShape(false);
	}

	// Dessiner les points de controle et les lignes de controle
	ofSetLineWidth(1);
	ofSetColor(s.contourColor, 150);

	// Lignes entre les points de controle
	for (size_t i = 0; i < s.points.size() - 1; i++) {
		ofDrawLine(s.points[i], s.points[i + 1]);
	}

	// Points de controle
	ofFill();
	// Mauve par defaut pour les points de controle
	ofSetColor(255, 0, 255, 200);
	for (const auto& point : s.points) {
		ofDrawCircle(point, 3.0f);
	}

	ofPopMatrix();
}
