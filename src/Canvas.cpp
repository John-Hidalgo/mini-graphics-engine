#include "Canvas.h"
#include "Toolbar.h"
#include "SceneGraph.h"
#include "of3dUtils.h"
#include "Quadtree.h"


Canvas::Canvas() {}

void Canvas::setup(const ofRectangle& area,const ofRectangle& areaForCameras,Toolbar* toolbar,SceneGraph* sceneGraph){
	canvasLight.setPointLight();
	canvasLight.setDiffuseColor(255);
	canvasLight.setGlobalPosition(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 255.0f);
	toolbarRef = toolbar;
	sceneGraphRef = sceneGraph;
	drawingArea = area;
	drawingAreaForCameras = areaForCameras;
	ofSetLineWidth(2);
	model3D.setup();
	color_picker_background.set("background color", ofColor(255, 255, 0), ofColor(0, 0), ofColor(255, 255));
	color_picker_ambient.set("ambient color", ofColor(63, 63, 63), ofColor(0, 0), ofColor(255, 255));
	color_picker_diffuse.set("diffuse color", ofColor(174, 223, 134), ofColor(0, 0), ofColor(255, 255));
//	ofLogNotice() << "drawingArea: x=" << drawingArea.x
//				  << " y=" << drawingArea.y
//				  << " w=" << drawingArea.getWidth()
//				  << " h=" << drawingArea.getHeight()
//				  << " bottom=" << drawingArea.getBottom()
//				  << " window height=" << ofGetHeight();
	tempPrimitive.setup();
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

			// 7.2 - Pour le material:
			model->material.setAmbientColor(sceneGraphRef->material_ambient_color_model.get());
			model->material.setDiffuseColor(sceneGraphRef->material_diffuse_color_model.get());
			model->material.setEmissiveColor(sceneGraphRef->material_emissive_color_model.get());
			model->material.setSpecularColor(sceneGraphRef->material_specular_color_model.get());
			model->material.setShininess(sceneGraphRef->material_shininess_model);
		}
	}
	else if (!sceneGraphRef->selectedPrimitiveIndices.empty()) {
		for (int idx : sceneGraphRef->selectedPrimitiveIndices) {
			auto& primitive3D = primitives3D[idx];
			primitive3D.size = sceneGraphRef->primitives3DSizeSlider;
			primitive3D.position = ofVec3f(sceneGraphRef->primitives3DPosXSlider, sceneGraphRef->primitives3DPosYSlider, sceneGraphRef->primitives3DPosZSlider);
			primitive3D.color = sceneGraphRef->color_picker_background_primitives3D;
			primitive3D.color_ambient = sceneGraphRef->color_picker_ambient_primitives3D;
			primitive3D.color_diffuse = sceneGraphRef->color_picker_diffuse_primitives3D;

			// 7.2 - Pour le material:
			primitive3D.material.setAmbientColor(sceneGraphRef->material_ambient_color_primitives3D.get());
			primitive3D.material.setDiffuseColor(sceneGraphRef->material_diffuse_color_primitives3D.get());
			primitive3D.material.setEmissiveColor(sceneGraphRef->material_emissive_color_primitives3D.get());
			primitive3D.material.setSpecularColor(sceneGraphRef->material_specular_color_primitives3D.get());
			primitive3D.material.setShininess(sceneGraphRef->material_shininess_primitives3D);
			primitive3D.generateMesh();
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

	for (auto& l : lights) {
		if (!l.enabled) continue;

		if (l.type == LightType::LIGHT_DIRECTIONAL) {
			l.light->lookAt(l.direction);
		} else {
			l.light->setPosition(l.position);
		}
		l.light->setDiffuseColor(l.color);
	}
}
void Canvas::drawCanvas(){
	if(hasImage){
		ofSetColor(bgColor);
	}
	else if(skyBoxDisplayed){
		ofSetColor(bgColor.r, bgColor.g, bgColor.b, 0);
	}
	else{
		ofSetColor(bgColor);
	}
	ofFill();
	ofDrawRectangle(drawingArea);
	ofNoFill();
}
void Canvas::draw2d() {
	ofPushStyle();

	drawCanvas();
	drawImage(drawingArea);
	drawImageThumbnails();

	Quadtree quadtree(drawingArea, 5);
	for (auto &s : shapes) {
		quadtree.insert(s);
	}

	std::vector<Shape> visibleShapes;
	quadtree.query(drawingArea, visibleShapes);

	for (auto &s : visibleShapes) {
		ShapeRenderer::drawShape(s);
	}
	ShapeRenderer::drawPreview(currentMode, start, end, currentColor, tempShape.points, drawing);

	if (hasImage) {
		if (showHistogram) {
			histogram.draw(drawingArea.getX() + 700, drawingArea.getY() + 900, 240, 80);
		}

		ofSetColor(0, 0, 0);
		ofDrawBitmapString(
			"Images: " + ofToString(importedImages.size()) +
			" | Courant: " + ofToString(currentImageIndex + 1),
			drawingArea.x + 10, drawingArea.y + 20
		);

		ofDrawBitmapString(
			"GAUCHE/DROITE : Parcourir les images | RETOUR ARRIÈRE : Supprimer l'image actuelle",
			drawingArea.x + 10, drawingArea.y + 40
		);
	}

	quadtree.draw();
	ofPopStyle();
}
void Canvas::draw2DInViewport(const ofRectangle& viewport) {
	ofPushStyle();
	ofSetColor(bgColor);
	ofFill();
	if(hasImage){
		ofSetColor(bgColor);
	}
	else if(skyBoxDisplayed){
		ofSetColor(bgColor.r, bgColor.g, bgColor.b, 0);
	}
	else{
		ofSetColor(bgColor);
	}
	ofFill();
	ofDrawRectangle(drawingAreaForCameras);
	ofNoFill();
	drawImage(drawingAreaForCameras);
	for (auto &s : shapes) {
		ShapeRenderer::drawShape(s);
	}
	ShapeRenderer::drawPreview(currentMode, start, end, currentColor, tempShape.points, drawing);
	ofPopStyle();
}

void Canvas::draw3d(){
	for (auto& m : models) {
		m->showBoundingBox = showBoundingBoxes;
		m->draw({});
	}

	// Dessins des primitives 3D pre-existantes
	drawPrimitives3D();

	// TEMP
	// Dessin de la primtive en train d'etre tracé a la souris
	drawPrimitivePreview();

	for (auto& l : lights) {
		if (l.enabled) l.light->enable();
	}

	for (auto& l : lights) {
		ofPushMatrix();
		ofTranslate(l.position);
		switch (l.type) {
		case LightType::LIGHT_DIRECTIONAL:
			ofSetColor(ofColor::orange);
			ofDrawArrow(glm::vec3(0), l.direction * 30.0f, 3);
			break;
		case LightType::LIGHT_POINT:
			ofSetColor(ofColor::yellow);
			ofDrawSphere(0, 0, 0, 5);
			break;
		case LightType::LIGHT_SPOT:
			ofSetColor(ofColor::green);
			ofDrawCone(0, 0, 0, 5, 15);
			break;
		case LightType::LIGHT_AMBIENT:
			ofSetColor(ofColor::gray);
			ofDrawBox(0, 0, 0, 4);
			break;
		}
		ofPopMatrix();
	}
}
void Canvas::draw() {
	draw2d();
	draw3d();
}

void Canvas::mousePressed(int x, int y, int button) {
	ofPushStyle();
	if (placingLight) {
		glm::vec3 worldPos = customScreenToWorld(x, y, 0);
		addLight(currentLightType, worldPos);
		placingLight = false;
		return;
	}
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
		//tempPrimitive.setup();
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
					(currentMode == ShapeMode::HOUSE      && dist < 1) ||
						(currentMode == ShapeMode::TREE      && dist < 1) ||
							(currentMode == ShapeMode::TARGET      && dist < 1) ||
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

		float canvasWidth = area.getWidth();
		float canvasHeight = area.getHeight();
		float imageWidth = currentImage.getWidth();
		float imegeHeight = currentImage.getHeight();

		// On calcule le scale de base sans param du user
		float baseScaleX = canvasWidth / imageWidth;
		float baseScaleY = canvasHeight / imegeHeight;
		float baseScale = std::min(baseScaleX, baseScaleY);

		// On applique le display scale venant du slider de sample
		float finalScale = baseScale * imageDisplayScale;

		float drawWidth, drawHeight;

		if (keepAspectRatio) {
			// On garde le aspect ratio d'origine
			drawWidth = imageWidth * finalScale;
			drawHeight = imegeHeight * finalScale;
		} else {
			// On permet de la distortion potentielle en laissant un aspect ratio libre
			drawWidth = imageWidth * baseScaleX * imageDisplayScale;
			drawHeight = imegeHeight * baseScaleY * imageDisplayScale;
		}

		// On calcule la position avec un offset
		float drawX = area.x + (canvasWidth - drawWidth) / 2 + imageDisplayOffset.x;
		float drawY = area.y + (canvasHeight - drawHeight) / 2 + imageDisplayOffset.y;

		ofSetColor(255, 255, 255);
		currentImage.draw(drawX, drawY, drawWidth, drawHeight);
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
	if(hasModel) model3D.draw(lights);
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
	primitive.setup(); 
	primitive.size = size;
	primitive.generateMesh();
	primitives3D.push_back(primitive);
}

void Canvas::drawPrimitives3D() {
	for (auto& primitive : primitives3D) {
		primitive.draw(canvasLight, showBoundingBoxes, lights);
	}
}

void Canvas::drawPrimitivePreview() {
	if (drawingPrimitive && currentPrimitiveMode != Primitive3DType::NONE) {
		tempPrimitive.draw(canvasLight);  // Same simple call for preview
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

void Canvas::addLight(LightType type, const glm::vec3& position) {
	lights.emplace_back(type);
	LightData& data = lights.back();

	data.light = std::make_unique<ofLight>();
	data.position = position;

	switch (type) {
	case LightType::LIGHT_AMBIENT:
		data.light->setAmbientColor(ofFloatColor(0.2, 0.2, 0.2));
		break;

	case LightType::LIGHT_DIRECTIONAL:
		data.light->setDirectional();
		data.light->setOrientation({45, 0, 0});
		data.light->setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
		break;

	case LightType::LIGHT_POINT:
		data.light->setPointLight();
		data.light->setPosition(position);
		data.light->setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
		break;

	case LightType::LIGHT_SPOT:
		data.light->setSpotlight();
		data.light->setPosition(position);
		data.light->setSpotlightCutOff(45);
		data.light->setSpotConcentration(60);
		data.light->setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
		break;
	}

	data.light->enable();
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
