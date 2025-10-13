#include "Canvas.h"
#include "Toolbar.h"
#include "SceneGraph.h"
#include "of3dUtils.h"

Canvas::Canvas() {}

void Canvas::setup(const ofRectangle& area,const ofRectangle& areaForCameras,Toolbar* toolbar,SceneGraph* sceneGraph){
	toolbarRef = toolbar;
	sceneGraphRef = sceneGraph;
	drawingArea = area;
	drawingAreaForCameras = areaForCameras;
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

// On en a besoin pour transformer les clics sur l'ecran en position 3D dans l'editeur
void Canvas::setActiveCamera(ofCamera* cam, const ofRectangle& viewport) {
	activeCamera = cam;
	cameraViewport = viewport;
}

ofPoint Canvas::customScreenToWorld(int x, int y, float planeZ) {
	if (!activeCamera) return ofPoint(0, 0, 0);

	// Convert screen coordinates to viewport-relative coordinates
	float viewportX = x - cameraViewport.x;
	float viewportY = y - cameraViewport.y;

	// Use the camera's screenToWorld method
	ofPoint screenPoint(viewportX, viewportY, 0);
	ofPoint worldPoint = activeCamera->screenToWorld(screenPoint, cameraViewport);

	return worldPoint;
}

void Canvas::update() {
	if (hasModel) {
		model3D.update();

		model3D.color_background = color_picker_background;
		if (model3D.variant == ModelVariant::None) {
			model3D.color_ambient = color_picker_ambient;
			model3D.color_diffuse = color_picker_diffuse;
		}
	}

	calculateModelsPosition();

	if (!sceneGraphRef->selectedModelIndices.empty()) {
		for (int idx : sceneGraphRef->selectedModelIndices) {
			auto& model = models[idx];
			model->color_background = sceneGraphRef->color_picker_background;
			model->scale_model = sceneGraphRef->scaleSlider;

			if (model->variant == ModelVariant::None) {
				model->color_ambient = sceneGraphRef->color_picker_ambient;
				model->color_diffuse = sceneGraphRef->color_picker_diffuse;
			}
		}
	} else {
		for (auto &model : models) {
			model->color_background = sceneGraphRef->color_picker_background;
			model->scale_model = sceneGraphRef->scaleSlider;

			if (model->variant == ModelVariant::None) {
				model->color_ambient = sceneGraphRef->color_picker_ambient;
				model->color_diffuse = sceneGraphRef->color_picker_diffuse;
			}
		}
	}
}
void Canvas::drawCanvas(){
	if(hasImage){
		ofSetColor(bgColor);
	}
	else{
		ofSetColor(bgColor);
	}
	ofFill();
	ofDrawRectangle(drawingArea);
	ofNoFill();
}
void Canvas::draw2d(){
	ofPushStyle();
	drawCanvas();
	drawImage(drawingArea);
	drawImageThumbnails();
	for (auto &s : shapes) {
		drawShape(s);
	}
	drawPreview();
	if (hasImage) {
			if (showHistogram) {
				histogram.draw(drawingArea.getX() + 650, drawingArea.getY() + 825, 300, 150);
			}
			ofSetColor(0, 0, 0);
			ofDrawBitmapString("Images: " + ofToString(importedImages.size()) +
							  " | Courant: " + ofToString(currentImageIndex + 1),
							  drawingArea.x + 10, drawingArea.y + 20);
			
			ofDrawBitmapString("GAUCHE/DROITE : Parcourir les images | RETOUR ARRIÈRE : Supprimer l'image actuelle",
							  drawingArea.x + 10, drawingArea.y + 40);
		}
	ofPopStyle();
}
void Canvas::draw2DInViewport(const ofRectangle& viewport) {
	ofPushStyle();
	ofSetColor(bgColor);
	ofFill();
	if(hasImage){
		ofSetColor(bgColor);
	}
	else{
		ofSetColor(bgColor);
	}
	ofFill();
	ofDrawRectangle(drawingAreaForCameras);
	ofNoFill();
	drawImage(drawingAreaForCameras);
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

	// Dessins des primitives 3D pre-existantes
	drawPrimitives3D();

	// TEMP
	// Dessin de la primtive en train d'etre tracé a la souris
	drawPrimitivePreview();
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
			case ShapeMode::POINT: {
						float pointSize = 5.0;
						ofDrawCircle(start.x, start.y, pointSize);
						break;
			}
			case ShapeMode::SQUARE: {
						float x = std::min(start.x, end.x);
						float y = std::min(start.y, end.y);
						float w = std::abs(end.x - start.x);
						float h = std::abs(end.y - start.y);
						float size = std::min(w, h);
						if (end.x < start.x) x = start.x - size;
						if (end.y < start.y) y = start.y - size;
						ofDrawRectangle(x, y, size, size);
						break;
			}
			case ShapeMode::TRIANGLE: {
						ofPoint p1 = start;
						ofPoint p2 = ofPoint(end.x, start.y);
						ofPoint p3 = ofPoint((start.x + end.x) / 2, end.y);
						ofDrawTriangle(p1, p2, p3);
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
void Canvas::drawPoint(const Shape& s) {
	float pointSize = 5.0f;

	ofFill();
	ofSetColor(s.fillColor);
	ofDrawCircle(s.start, pointSize);

	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawCircle(s.start, pointSize);
}
void Canvas::drawTriangle(const Shape& s) {
	ofPoint p1 = s.start;
	ofPoint p2 = ofPoint(s.end.x, s.start.y);
	ofPoint p3 = ofPoint((s.start.x + s.end.x) / 2, s.end.y);

	// Fill
	ofFill();
	ofSetColor(s.fillColor);
	ofDrawTriangle(p1, p2, p3);

	// Outline
	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawTriangle(p1, p2, p3);
}
void Canvas::drawSquare(const Shape& s) {
	float w = std::abs(s.end.x - s.start.x);
	float h = std::abs(s.end.y - s.start.y);
	float size = std::min(w, h);
	float x = s.start.x;
	float y = s.start.y;
	if (s.end.x < s.start.x) x = s.start.x - size;
	if (s.end.y < s.start.y) y = s.start.y - size;
	ofFill();
	ofSetColor(s.fillColor);
	ofDrawRectangle(x, y, size, size);
	ofNoFill();
	ofSetLineWidth(s.thickness);
	ofSetColor(s.contourColor);
	ofDrawRectangle(x, y, size, size);
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
		case ShapeMode::POINT: drawPoint(s); break;
		case ShapeMode::TRIANGLE: drawTriangle(s); break;
		case ShapeMode::SQUARE: drawSquare(s); break;
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

	if (currentPrimitiveMode != Primitive3DType::NONE) {
		// Début du dessin d'une primitive 3D
		drawingPrimitive = true;

		// On transforme les coordonnees 2D du click sur l'ecran en coordonnees 3D dans l'editeur
		primitiveStartPos = customScreenToWorld(x + 224.0f, y, 0.0f);

		// Construction d'une primitive temporaire pour voir ce que l'on dessine
		tempPrimitive.type = currentPrimitiveMode;
		tempPrimitive.position = primitiveStartPos;
		tempPrimitive.color = currentColor;

		// Taille minimale affichable
		tempPrimitive.size = 10.0f;
		tempPrimitive.generateMesh();
        std::cout << "mousePressed -> Primitives 3D Start Position: " << primitiveStartPos << std::endl;

		return;
	}

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
	if (drawingPrimitive) {
		end.set(x, y);

		// On update la taille primitive temp de preview
		tempPrimitive.setSizeFromPoints(start, end);
    } else if (drawing) {
		end.set(x, y);
		if (currentMode == ShapeMode::FREEFORM) {
			tempShape.points.push_back(ofPoint(x, y));
		}
	}
}

void Canvas::mouseReleased(int x, int y, int button) {
	if (!drawingArea.inside(x, y)) return;

	if (drawingPrimitive) {
		end.set(x, y);
		drawingPrimitive = false;

		float distance = start.distance(end);

		// On crée la primitive seulement si elle est assez grande
		if (distance > 5.0f) {
			addPrimitive3D(currentPrimitiveMode, primitiveStartPos, distance);
		}

	} else if (drawing) {
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
				(currentMode == ShapeMode::POINT      && dist < 1) ||
				(currentMode == ShapeMode::SQUARE      && (dx < 1 && dy < 1)) ||
				(currentMode == ShapeMode::TRIANGLE      && dist < 1) ||
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
void Canvas::setCanvasAreaForCameras(const ofRectangle& area) {
	drawingArea = area;
}
void Canvas::drawImage(const ofRectangle& area) {
	if (hasImage && currentImageIndex >= 0 && currentImageIndex < importedImages.size()) {
		ofImage& currentImage = importedImages[currentImageIndex];
		
		float canvasW = area.getWidth();
		float canvasH = area.getHeight();
		float imgW = currentImage.getWidth();
		float imgH = currentImage.getHeight();

		float scaleX = canvasW / imgW;
		float scaleY = canvasH / imgH;
		float scale = std::min(scaleX, scaleY);

		float drawW = imgW * scale;
		float drawH = imgH * scale;

		float drawX = area.x + (canvasW - drawW) / 2;
		float drawY = area.y + (canvasH - drawH) / 2;

		ofSetColor(255, 255, 255);
		currentImage.draw(drawX, drawY, drawW, drawH);
	}
}

void Canvas::loadImage(const std::string &path) {
	ofImage newImage;
	if (newImage.load(path)) {
		importedImages.push_back(newImage);
		imagePaths.push_back(path);
		hasImage = true;
		
		currentImageIndex = importedImages.size() - 1;
		histogram.calculateColours(importedImages[currentImageIndex]);
		
		ofLogNotice() << "Loaded image: " << path << " (Total: " << importedImages.size() << ")";
		ofLogNotice() << "Current image index: " << currentImageIndex;
	} else {
		ofLogError() << "Failed to load image: " << path;
	}
}

void Canvas::loadModel(const std::string& path) {
	model3D.loadModel(path);
	hasModel = true;
}
void Canvas::drawModel() {
	if(hasModel) model3D.draw();
}
//void Canvas::calculateModelsPosition() {
//	int n = models.size();
//	if (n == 0) return;
//
//	float radius = 50.0f;
//	radius = sceneGraphRef->positionSlider;
//	float centerX = 0;
//	float centerY = 0;//drawingArea.y + drawingArea.height * 0.5f;
//
//	for (int i = 0; i < n; i++) {
//		float angle = TWO_PI * i / n;
//		float x = centerX + radius * cos(angle);
//		float y = centerY + radius * sin(angle);
//
//		models[i]->position.set(x, y, 0);
//		models[i]->update();
//	}
//}
void Canvas::calculateModelsPosition() {
	int n = models.size();
	if (n == 0) return;

	float radius = sceneGraphRef->positionSlider;
	float centerX = 0;
	float centerZ = 0;

	//ofLog() << "=== Calculating positions for " << n << " models ===";
	
	for (int i = 0; i < n; i++) {
		float angle = TWO_PI * i / n;
		float x = centerX + radius * cos(angle);
		float z = centerZ + radius * sin(angle);

		models[i]->position.set(x, 0, z);
		models[i]->update();
		
		//ofLog() << "Model " << i << " position set to: "
				//<< "x=" << x << ", y=0, z=" << z;
		//ofLog() << "Model " << i << " actual position: "
				//<< models[i]->position;
	}
}

void Canvas::setCurrentPrimitiveMode(Primitive3DType mode) {
	currentPrimitiveMode = mode;
}

void Canvas::addPrimitive3D(Primitive3DType type, const ofPoint& position, float size) {
	Primitive3D primitive;
	primitive.type = type;
	primitive.position = position;
	primitive.color = currentColor;
	primitive.size = size;
	primitive.generateMesh();
	primitives3D.push_back(primitive);
}

void Canvas::drawPrimitives3D() {
	for (auto& primitive : primitives3D) {
		ofPushMatrix();
		ofTranslate(primitive.position);

		if (primitive.isSelected) {
			// Si la primitive sélectionné on la met en jaune
			ofSetColor(255, 255, 0);
		} else {
			ofSetColor(primitive.color);
		}

		// On dessine la primitive 3D
        primitive.mesh.draw();
		ofPopMatrix();
	}
}

void Canvas::drawPrimitivePreview() {
	if (drawingPrimitive && currentPrimitiveMode != Primitive3DType::NONE) {
		ofPushMatrix();
		ofTranslate(tempPrimitive.position);
		ofSetColor(currentColor);
		tempPrimitive.mesh.draw();
		ofPopMatrix();
  }
}

void Canvas::loadMultipleImages(const std::vector<std::string>& paths) {
	for (const auto& path : paths) {
		loadImage(path);
	}
}

void Canvas::setCurrentImage(int index) {
	if (index >= 0 && index < importedImages.size()) {
		ofImage selectedImage = importedImages[index];
		std::string selectedPath = imagePaths[index];
		importedImages.erase(importedImages.begin() + index);
		imagePaths.erase(imagePaths.begin() + index);
		importedImages.push_back(selectedImage);
		imagePaths.push_back(selectedPath);
		histogram.calculateColours(importedImages.back());
		
		//ofLogNotice() << "Set current image to index: " << index << " (now at position: " << getCurrentImageIndex() << ")";
	}
}

void Canvas::removeImage(int index) {
	if (index >= 0 && index < importedImages.size()) {
		importedImages.erase(importedImages.begin() + index);
		imagePaths.erase(imagePaths.begin() + index);
		hasImage = !importedImages.empty();
		if (hasImage) {
			histogram.calculateColours(importedImages.back());
		}
		
		//ofLogNotice() << "Removed image at index: " << index << " (Remaining: " << importedImages.size() << ")";
	}
}

void Canvas::clearAllImages() {
	importedImages.clear();
	imagePaths.clear();
	hasImage = false;
	//ofLogNotice() << "Cleared all images";
}

ofImage& Canvas::getCurrentImage() {
	if (hasImage && currentImageIndex >= 0 && currentImageIndex < importedImages.size()) {
		return importedImages[currentImageIndex];
	}
	static ofImage emptyImage;
	return emptyImage;
}

void Canvas::keyPressed(int key) {
	if (!hasImage) return;
	
	switch (key) {
		case OF_KEY_LEFT:
			previousImage();
			break;
		case OF_KEY_RIGHT:
			nextImage();
			break;
		case OF_KEY_BACKSPACE:
			deleteCurrentImage();
			break;
	}
}

void Canvas::nextImage() {
	if (importedImages.size() <= 1) return;
	
	currentImageIndex = (currentImageIndex + 1) % importedImages.size();
	histogram.calculateColours(importedImages[currentImageIndex]);
	
	ofLogNotice() << "Next image - Current index: " << currentImageIndex;
}

void Canvas::previousImage() {
	if (importedImages.size() <= 1) return;
	
	currentImageIndex = (currentImageIndex - 1 + importedImages.size()) % importedImages.size();
	histogram.calculateColours(importedImages[currentImageIndex]);
	
	ofLogNotice() << "Previous image - Current index: " << currentImageIndex;
}

void Canvas::deleteCurrentImage() {
	if (!hasImage || importedImages.empty()) return;
	
	ofLogNotice() << "Deleting current image at index: " << currentImageIndex;
	
	importedImages.erase(importedImages.begin() + currentImageIndex);
	imagePaths.erase(imagePaths.begin() + currentImageIndex);
	
	if (importedImages.empty()) {
		hasImage = false;
		currentImageIndex = -1;
	} else {
		currentImageIndex = currentImageIndex % importedImages.size();
		histogram.calculateColours(importedImages[currentImageIndex]);
	}
	
	ofLogNotice() << "Deleted image. Remaining: " << importedImages.size() << ", Current index: " << currentImageIndex;
}

void Canvas::drawImageThumbnails() {
	if (importedImages.size() <= 1) return;

	float thumbnailSize = 60.0f;
	float spacing = 5.0f;
	float startX = drawingArea.x + 20;
	float startY = drawingArea.getBottom() - thumbnailSize - 20;
	
	for (int i = 0; i < importedImages.size(); i++) {
		float x = startX + i * (thumbnailSize + spacing);
		float y = startY;
		if (i == currentImageIndex) {
			ofSetColor(100, 200, 255);
			ofDrawRectangle(x - 2, y - 2, thumbnailSize + 4, thumbnailSize + 4);
		} else {
			ofSetColor(150, 150, 150);
			ofDrawRectangle(x - 1, y - 1, thumbnailSize + 2, thumbnailSize + 2);
		}
		
		ofSetColor(255, 255, 255);
		importedImages[i].draw(x, y, thumbnailSize, thumbnailSize);
		
		ofSetColor(0, 0, 0);
		ofDrawBitmapString(ofToString(i + 1), x + 5, y + 15);
	}
}
