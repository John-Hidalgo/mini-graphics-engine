#include "Canvas.h"
#include "Toolbar.h"
#include "SceneGraph.h"
#include "of3dUtils.h"

Canvas::Canvas() {}

void Canvas::setup(const ofRectangle& area,Toolbar* toolbar,SceneGraph* sceneGraph){
	toolbarRef = toolbar;
	sceneGraphRef = sceneGraph;
	drawingArea = area;
	ofSetLineWidth(2);
	model3D.setup();
	color_picker_background.set("background color", ofColor(255, 255, 0), ofColor(0, 0), ofColor(255, 255));
	color_picker_ambient.set("ambient color", ofColor(63, 63, 63), ofColor(0, 0), ofColor(255, 255));
	color_picker_diffuse.set("diffuse color", ofColor(174, 223, 134), ofColor(0, 0), ofColor(255, 255));
	ofLogNotice() << "drawingArea: x=" << drawingArea.x
				  << " y=" << drawingArea.y
				  << " w=" << drawingArea.getWidth()
				  << " h=" << drawingArea.getHeight()
				  << " bottom=" << drawingArea.getBottom()
				  << " window height=" << ofGetHeight();

}

void Canvas::update() {
	if(hasModel) {
		model3D.update();
		model3D.color_background = color_picker_background;
		model3D.color_ambient = color_picker_ambient;
		model3D.color_diffuse = color_picker_diffuse;
	}
	calculateModelsPosition();

	if(!sceneGraphRef->selectedModelIndices.empty()) {
		for(int idx : sceneGraphRef->selectedModelIndices) {
			auto& model = models[idx];
			model->color_background = sceneGraphRef->color_picker_background;
			model->color_ambient = sceneGraphRef->color_picker_ambient;
			model->color_diffuse = sceneGraphRef->color_picker_diffuse;
			model->scale_model = sceneGraphRef->scaleSlider;
		}
	} else {
		for(auto &model : models) {
			model->color_background = sceneGraphRef->color_picker_background;
			model->color_ambient = sceneGraphRef->color_picker_ambient;
			model->color_diffuse = sceneGraphRef->color_picker_diffuse;
			model->scale_model = sceneGraphRef->scaleSlider;
		}
	}
}
void Canvas::drawCanvas(){
	if(hasImage){
		ofSetColor(255,255,255);
	}
	else{
		ofSetColor(255,255,255);
	}
	ofFill();
	ofDrawRectangle(drawingArea);
	ofNoFill();
}
void Canvas::draw2d(){
	ofPushStyle();
	drawCanvas();
	drawImage();

	for (auto &s : shapes) {
		drawShape(s);
	}
	drawPreview();
	ofPopStyle();
}
void Canvas::draw3d(){
	for (auto &m : models) {
		m->draw();
	}

	// TODO: Juste pour tester la camera
	ofDrawBox(-100, 0, 0, 100);
}

void Canvas::draw() {
	draw2d();
	draw3d();
}
void Canvas::drawPreview(){
	if (drawing) {
		ofSetColor(currentColor);
		ofNoFill();
		switch (currentMode) {
			case ShapeMode::RECTANGLE: {
				float x = std::min(start.x, end.x);
				float y = std::min(start.y, end.y);
				float w = std::abs(end.x - start.x);
				float h = std::abs(end.y - start.y);
				ofDrawRectangle(x, y, w, h);
				break;
			}
			case ShapeMode::CIRCLE: {
				float radius = start.distance(end);
				ofDrawCircle(start, radius);
				break;
			}
			case ShapeMode::LINE: {
				ofDrawLine(start, end);
				break;
			}
			case ShapeMode::FREEFORM: {
				ofBeginShape();
				for (auto &p : tempShape.points) {
					ofVertex(p.x, p.y);
				}
				ofEndShape(false);
				break;
			}
			default: break;
		}
	}
}
void Canvas::drawRectangle(const Shape& s) {
	float x = std::min(s.start.x, s.end.x);
	float y = std::min(s.start.y, s.end.y);
	float w = std::abs(s.end.x - s.start.x);
	float h = std::abs(s.end.y - s.start.y);

	ofFill();
	ofSetColor(s.fillColor);
	ofDrawRectangle(x, y, w, h);

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawRectangle(x, y, w, h);
}
void Canvas::drawCircle(const Shape& s) {
	float radius = s.start.distance(s.end);

	ofFill();
	ofSetColor(s.fillColor);
	ofDrawCircle(s.start, radius);

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawCircle(s.start, radius);
}
void Canvas::drawFreeForm(const Shape& s) {
	if (!s.points.empty()) {
		ofFill();
		ofSetColor(s.fillColor);
		ofBeginShape();
		for (auto& p : s.points) {
			ofVertex(p.x, p.y);
		}
		ofEndShape(true);
	}

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofBeginShape();
	for (auto& p : s.points) {
		ofVertex(p.x, p.y);
	}
	ofEndShape(false);
}

void Canvas::drawShape(const Shape& s) {
	switch (s.type) {
		case ShapeMode::RECTANGLE: drawRectangle(s); break;
		case ShapeMode::CIRCLE: drawCircle(s); break;
		case ShapeMode::LINE: {
			ofSetLineWidth(s.thickness);
			ofSetColor(s.contourColor);
			ofDrawLine(s.start, s.end);
			break;
		}
		case ShapeMode::FREEFORM: drawFreeForm(s); break;
		default: break;
	}
}
void Canvas::mousePressed(int x, int y, int button) {
	ofPushStyle();
	if (!drawingArea.inside(x, y)) return;
	
	if (toolbarRef && toolbarRef->isSelectingColor()) {
		
		ofPixels pixels;
		ofImage screen;
		screen.grabScreen(x, y, 1, 1);
		pixels = screen.getPixels();
		ofColor picked = pixels.getColor(0, 0);
		currentColor = picked;
		if (toolbarRef) {
			std::string nom = "Canevas";
			toolbarRef->setColorFromCanvas(picked,nom);
			setDrawingColor(picked);
		}
		return;
	}
	start.set(x, y);
	end.set(x, y);
	drawing = true;
	ofNoFill();

	if (currentMode == ShapeMode::FREEFORM) {
		tempShape = Shape();
		tempShape.type = ShapeMode::FREEFORM;
		tempShape.contourColor = currentColor;
		tempShape.points.push_back(ofPoint(x, y));
	}
	ofPopStyle();
}


void Canvas::mouseDragged(int x, int y, int button) {
	if (!drawingArea.inside(x, y)) return;
	if (drawing) {
		end.set(x, y);
		if (currentMode == ShapeMode::FREEFORM) {
			tempShape.points.push_back(ofPoint(x, y));
		}
	}
}

void Canvas::mouseReleased(int x, int y, int button) {
	if (!drawingArea.inside(x, y)) return;

	if (drawing) {
		end.set(x, y);
		drawing = false;

		if (currentMode == ShapeMode::NONE) return;

		float dx = std::abs(end.x - start.x);
		float dy = std::abs(end.y - start.y);
		float dist = start.distance(end);

		bool isTrivial =
			(currentMode == ShapeMode::RECTANGLE && (dx < 1 && dy < 1)) ||
			(currentMode == ShapeMode::CIRCLE    && dist < 1) ||
			(currentMode == ShapeMode::LINE      && dist < 1) ||
			(currentMode == ShapeMode::FREEFORM  && tempShape.points.size() < 2);

		if (isTrivial) return;

		Shape s;
		s.type = currentMode;
		s.start = start;
		s.end = end;
		s.contourColor = currentColor;
		s.thickness = 2.0f;

		if (currentMode == ShapeMode::FREEFORM) {
			s.points = tempShape.points;
		}

		shapes.push_back(s);
	}
}

void Canvas::undo() {
	if (!shapes.empty()) {
		shapes.pop_back();
	}
}

void Canvas::clear() {
	shapes.clear();
	drawing = false;
}

void Canvas::setDrawingArea(const ofRectangle& area) {
	drawingArea = area;
}
void Canvas::drawImage() {
	if (hasImage) {
		float canvasW = drawingArea.getWidth();
		float canvasH = drawingArea.getHeight();
		float imgW = importedImage.getWidth();
		float imgH = importedImage.getHeight();

		float scaleX = canvasW / imgW;
		float scaleY = canvasH / imgH;
		float scale = std::min(scaleX, scaleY);

		float drawW = imgW * scale;
		float drawH = imgH * scale;

		float drawX = drawingArea.x + (canvasW - drawW) / 2;
		float drawY = drawingArea.y + (canvasH - drawH) / 2;

		importedImage.draw(drawX, drawY, drawW, drawH);
	}
}

void Canvas::loadImage(const std::string & path) {
	if (importedImage.load(path)) {
		hasImage = true;
	} else {
		ofLogError() << "Failed to load image: " << path;
		hasImage = false;
	}
}

void Canvas::loadModel(const std::string& path) {
	model3D.loadModel(path);
	hasModel = true;
}
void Canvas::drawModel() {
	if(hasModel) model3D.draw();
}
void Canvas::calculateModelsPosition() {
	int n = models.size();
	if (n == 0) return;

	float radius = 50.0f;
	radius = sceneGraphRef->positionSlider;
	float centerX = 0;//drawingArea.x + drawingArea.width * 0.5f;
	float centerY = 0;//drawingArea.y + drawingArea.height * 0.5f;

	for (int i = 0; i < n; i++) {
		float angle = TWO_PI * i / n;
		float x = centerX + radius * cos(angle);
		float y = centerY + radius * sin(angle);

		models[i]->position.set(x, y, 0);
		models[i]->update();
	}
}
