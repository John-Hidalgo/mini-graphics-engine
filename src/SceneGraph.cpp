#include "SceneGraph.h"

SceneGraph::SceneGraph()
	: x(800), y(0), width(224), height(768), panelPadding(10)
{}

void SceneGraph::setup(Canvas* canvas, const ofRectangle& area) {
	canvasRef = canvas;
	panelArea = area;

	x = area.x;
	y = area.y;
	width = area.width;
	height = area.height;

	gui.setup("Graphe Du Scene");
	gui.setPosition(x + panelPadding, y + panelPadding);
	gui.setSize(175,0);

	gui.setBackgroundColor(ofColor(50, 50, 50));
	gui.setBorderColor(ofColor(100, 100, 100));
	gui.setTextColor(ofColor(255, 255, 255));
	gui.setHeaderBackgroundColor(ofColor(50, 50, 50));

	thicknessSlider.setup("Epaisseur", 2.0f, 1.0f, 20.0f);
	thicknessSlider.addListener(this, &SceneGraph::thicknessChanged);
	gui.add(&thicknessSlider);
	
	contourToggle.setup("Contour", true);
	contourToggle.addListener(this, &SceneGraph::contourToggled);
	gui.add(&contourToggle);
	
	backgroundToggle.setup("Fond", false);
	backgroundToggle.addListener(this, &SceneGraph::backgroundToggled);
	gui.add(&backgroundToggle);

	fillToggle.setup("Zone du remplissage", false);
	fillToggle.addListener(this, &SceneGraph::fillToggled);
	gui.add(&fillToggle);

	gui.add(colour2dShapes);
	colour2dShapes.set("Couleur", ofColor(0,0,0), ofColor(0,0,0), ofColor(255,255,255));
	colour2dShapes.addListener(this, &SceneGraph::colorChanged);
	gui.add(hueSlider.setup("Hue", 128, 0, 255));
	gui.add(satSlider.setup("Saturation", 200, 0, 255));
	gui.add(briSlider.setup("Brightness", 200, 0, 255));
	hueSlider.addListener(this, &SceneGraph::hsbChanged);
	satSlider.addListener(this, &SceneGraph::hsbChanged);
	briSlider.addListener(this, &SceneGraph::hsbChanged);

	deleteButton.setup("Effacez");
	deleteButton.addListener(this, &SceneGraph::deleteButtonPressed);
	gui.add(&deleteButton);
	
	modelEditorPanel.setup("Editez modeles");
	modelEditorPanel.setPosition(x + panelPadding + 175, y + panelPadding);
	modelEditorPanel.setSize(175,0);
	
	modelEditorPanel.add(scaleSlider.setup("Scale", 1.0f, 0.1f, 3.0f));
	modelEditorPanel.add(positionSlider.setup("Position", 100.0f, 0.1f, 700.0f));
	
	//modelEditorPanel.add(color_picker_background.set("background color", ofColor(15, 15, 15), ofColor(0, 0), ofColor(255,255)));
	modelEditorPanel.add(color_picker_ambient.set("ambient color", ofColor(63, 63, 63), ofColor(0, 0), ofColor(255, 255)));
	modelEditorPanel.add(color_picker_diffuse.set("diffuse color", ofColor(174, 223, 134), ofColor(0, 0), ofColor(255, 255)));
	
	textureGroup.setup("textures");
	textureInversionButton.setup("Inversion du cercle");
	textureWeierstrassButton.setup("Weierstrass");
	textureNormalMappingButton.setup("Effet De relief");
	textureInversionButton.addListener(this, &SceneGraph::textureInversionPressed);
	textureWeierstrassButton.addListener(this, &SceneGraph::textureWeierstrassPressed);
	textureNormalMappingButton.addListener(this, &SceneGraph::textureNormalMappingPressed);
	textureGroup.add(&textureInversionButton);
	textureGroup.add(&textureWeierstrassButton);
	textureGroup.add(&textureNormalMappingButton);
	modelEditorPanel.add(&textureGroup);
	
	animationGroup.setup("Animations");
	animationSurfaceToggle.setup("Animez la surface", false);
	animationColourToggle.setup("Animez le couleur", false);
	animationSurfaceToggle.addListener(this, &SceneGraph::animateSurfacePressed);
	animationColourToggle.addListener(this, &SceneGraph::animateColourPressed);
	animationGroup.add(&animationSurfaceToggle);
	animationGroup.add(&animationColourToggle);
	animationGroup.minimize();
	modelEditorPanel.add(&animationGroup);
	
	deleteButton3DModel.setup("Effacez");
	deleteButton3DModel.addListener(this,&SceneGraph::deleteButton3DModelPressed);
	modelEditorPanel.add(&deleteButton3DModel);
	
	gui.add(translateShapeXSlider.setup("Position X", 0, -500, 500));
	gui.add(translateShapeYSlider.setup("Position Y", 0, -500, 500));
	gui.add(scaleShapeSlider.setup("Proportion", 1.0f, 0.1f, 5.0f));
		
	rotateShapeRightButton.setup("Rotation Droite");
	rotateShapeLeftButton.setup("Rotation Gauche");
	gui.add(&rotateShapeRightButton);
	gui.add(&rotateShapeLeftButton);
		
	translateShapeXSlider.addListener(this, &SceneGraph::translationShapeChanged);
	translateShapeYSlider.addListener(this, &SceneGraph::translationShapeChanged);
	scaleShapeSlider.addListener(this, &SceneGraph::scaleShapeChanged);
	rotateShapeRightButton.addListener(this, &SceneGraph::rotateShapeRightPressed);
	rotateShapeLeftButton.addListener(this, &SceneGraph::rotateShapeLeftPressed);

	primitives3DEditorPanel.setup("Editez Primitives 3D");
	primitives3DEditorPanel.setPosition(x + panelPadding + 350, y + panelPadding);
	primitives3DEditorPanel.setSize(175,0);

	primitives3DEditorPanel.add(primitives3DSizeSlider.setup("Size", 100.0f, 1.0f, 300.0f));
	primitives3DEditorPanel.add(primitives3DPosXSlider.setup("Position X", 100, 0, 1000));
	primitives3DEditorPanel.add(primitives3DPosYSlider.setup("Position Y", 100, 0, 1000));
	primitives3DEditorPanel.add(primitives3DPosZSlider.setup("Position Z", 100, 0, 1000));

	primitives3DEditorPanel.add(color_picker_background_primitives3D.set("background color", ofColor(15, 15, 15), ofColor(0, 0), ofColor(255,255)));
	primitives3DEditorPanel.add(color_picker_ambient_primitives3D.set("ambient color", ofColor(63, 63, 63), ofColor(0, 0), ofColor(255, 255)));
	primitives3DEditorPanel.add(color_picker_diffuse_primitives3D.set("diffuse color", ofColor(174, 223, 134), ofColor(0, 0), ofColor(255, 255)));

	deleteButtonPrimitives3D.setup("Effacez");
	deleteButtonPrimitives3D.addListener(this,&SceneGraph::deleteButtonPrimitives3DPressed);
	primitives3DEditorPanel.add(&deleteButtonPrimitives3D);
	
	setupNormalMappingVisible(false);
}
void SceneGraph::setupNormalMappingVisible(bool visible) {
	enableNormalMapping = visible;
	textureGroup.clear();
	textureGroup.add(&textureInversionButton);
	textureGroup.add(&textureWeierstrassButton);
	if (visible) {
		textureGroup.add(&textureNormalMappingButton);
	}
	textureGroup.minimize();
}

void SceneGraph::draw() {
	//ofPushStyle();
	ofSetColor(50, 50, 50);
	ofFill();
	ofDrawRectangle(x, y, width, height);
	//ofPopStyle();
	gui.draw();
	drawShapeList();
	modelEditorPanel.draw();
	drawModelList();
	drawPrimitivesList();
	primitives3DEditorPanel.draw();
}

void SceneGraph::clearSelection() {
	selectedShapeIndices.clear();
}

void SceneGraph::selectShapesInArea(const ofRectangle& selectionRect) {
    if (!canvasRef) return;

    auto& shapes = canvasRef->getShapes();
    selectedShapeIndices.clear();

    for (int i = 0; i < shapes.size(); i++) {
        const auto& s = shapes[i];
        ofRectangle shapeBounds;

        // --- Basic shapes ---
        if (s.type == ShapeMode::RECTANGLE || s.type == ShapeMode::SQUARE ||
            s.type == ShapeMode::CIRCLE || s.type == ShapeMode::TRIANGLE ||
            s.type == ShapeMode::LINE) {

            float x1 = std::min(s.start.x, s.end.x);
            float x2 = std::max(s.start.x, s.end.x);
            float y1 = std::min(s.start.y, s.end.y);
            float y2 = std::max(s.start.y, s.end.y);
            shapeBounds.set(x1, y1, x2 - x1, y2 - y1);
        }

        // --- Point ---
        else if (s.type == ShapeMode::POINT) {
            shapeBounds.set(s.start.x - 3, s.start.y - 3, 6, 6);
        }

        // --- Freeform shapes (based on points) ---
        else if (!s.points.empty()) {
            float minX = s.points[0].x, maxX = s.points[0].x;
            float minY = s.points[0].y, maxY = s.points[0].y;
            for (auto& p : s.points) {
                minX = std::min(minX, p.x);
                maxX = std::max(maxX, p.x);
                minY = std::min(minY, p.y);
                maxY = std::max(maxY, p.y);
            }
            shapeBounds.set(minX, minY, maxX - minX, maxY - minY);
        }

        // --- Composite shapes ---
        else if (s.type == ShapeMode::HOUSE || s.type == ShapeMode::TREE || s.type == ShapeMode::TARGET) {
            float x1 = std::min(s.start.x, s.end.x);
            float x2 = std::max(s.start.x, s.end.x);
            float y1 = std::min(s.start.y, s.end.y);
            float y2 = std::max(s.start.y, s.end.y);
            shapeBounds.set(x1, y1, x2 - x1, y2 - y1);
        }

        else {
            continue;
        }

        bool intersecting = selectionRect.intersects(shapeBounds);
        ofLog() << "→ Intersecting: " << (intersecting ? "YES" : "NO");

        if (intersecting) {
            selectedShapeIndices.push_back(i);
        }
    }

	select3DObjectsInArea(selectionRect);
}

void SceneGraph::select3DObjectsInArea(const ofRectangle& selectionRect) {
    if (!canvasRef) return;

    selectedModelIndices.clear();
    selectedPrimitiveIndices.clear();

    // Access camera for projection
    ofCamera* cam = canvasRef->activeCamera;
    if (!cam) return;

    // --- Select 3D Models ---
    auto& models = canvasRef->getModels();
    for (int i = 0; i < models.size(); i++) {
        auto& m = models[i];
        ofVec3f min = m->bbox.min;
        ofVec3f max = m->bbox.max;

        // Get the 8 corners of the 3D bounding box
        std::vector<ofVec3f> corners = {
            {min.x, min.y, min.z}, {max.x, min.y, min.z},
            {min.x, max.y, min.z}, {max.x, max.y, min.z},
            {min.x, min.y, max.z}, {max.x, min.y, max.z},
            {min.x, max.y, max.z}, {max.x, max.y, max.z}
        };

        // Project them to 2D screen coordinates
        ofRectangle projectedBox;
        bool first = true;
        for (auto& c : corners) {
            glm::vec3 screenPt = cam->worldToScreen(c);
            if (first) {
                projectedBox.set(screenPt.x, screenPt.y, 0, 0);
                first = false;
            } else {
                projectedBox.growToInclude(screenPt.x, screenPt.y);
            }
        }

        if (selectionRect.intersects(projectedBox)) {
            selectedModelIndices.push_back(i);
        }
    }

    // --- Select 3D Primitives ---
    auto& primitives = canvasRef->getPrimitives3D();
    for (int i = 0; i < primitives.size(); i++) {
        auto& p = primitives[i];
        ofVec3f min = p.bbox.min;
        ofVec3f max = p.bbox.max;

        std::vector<ofVec3f> corners = {
            {min.x, min.y, min.z}, {max.x, min.y, min.z},
            {min.x, max.y, min.z}, {max.x, max.y, min.z},
            {min.x, min.y, max.z}, {max.x, min.y, max.z},
            {min.x, max.y, max.z}, {max.x, max.y, max.z}
        };

        ofRectangle projectedBox;
        bool first = true;
        for (auto& c : corners) {
            glm::vec3 screenPt = cam->worldToScreen(c);
            if (first) {
                projectedBox.set(screenPt.x, screenPt.y, 0, 0);
                first = false;
            } else {
                projectedBox.growToInclude(screenPt.x, screenPt.y);
            }
        }

        if (selectionRect.intersects(projectedBox)) {
            selectedPrimitiveIndices.push_back(i);
        }
    }
}

void SceneGraph::deleteButtonPressed() {
	auto& shapes = canvasRef->getShapes();
	if (!selectedShapeIndices.empty()) {
		std::sort(selectedShapeIndices.begin(), selectedShapeIndices.end(), std::greater<int>());
		
		for (int index : selectedShapeIndices) {
			if (index >= 0 && index < shapes.size()) {
				shapes.erase(shapes.begin() + index);
			}
		}
		selectedShapeIndices.clear();
	} else if (!shapes.empty()) {
		shapes.pop_back();
	}
}
void SceneGraph::deleteButton3DModelPressed() {
	auto& models = canvasRef->getModels();
	if (!selectedModelIndices.empty()) {
		std::sort(selectedModelIndices.begin(), selectedModelIndices.end(), std::greater<int>());
		
		for (int index : selectedModelIndices) {
			if (index >= 0 && index < models.size()) {
				models.erase(models.begin() + index);
			}
		}
		selectedModelIndices.clear();
	} else if (!models.empty()) {
		models.pop_back();
	}
}
void SceneGraph::thicknessChanged(float & val) {
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];
			s.thickness = val;
		}
	}
}

void SceneGraph::colorChanged(ofColor& col) {

	if (backgroundToggle) {
		// Change canvas background color
		if (canvasRef) {
			ofLog() << "calling setBackgroundColor from SceneGraph ColorChanged! ";
			canvasRef->setBackgroundColor(col);
		}
	} else {
		// Existing shape color logic
		for (int index : selectedShapeIndices) {
			if (index >= 0 && index < canvasRef->getShapes().size()) {
				auto& s = canvasRef->getShapes()[index];
				if (contourToggle) {
					s.contourColor = col;
				} else if (fillToggle) {
					s.fillColor = col;
					s.hasFill = true;
				}
			}
		}
	}

	hueSlider = col.getHue();
	satSlider = col.getSaturation();
	briSlider = col.getBrightness();
}

void SceneGraph::hsbChanged(float& val) {
	ofColor c = ofColor::fromHsb(hueSlider, satSlider, briSlider);
	
	if (backgroundToggle) {
		if (canvasRef) {
			//ofLog() << "calling setBackgroundColor from SceneGraph hsbChanged! ";
		}
	} else {
		for (int index : selectedShapeIndices) {
			if (index >= 0 && index < canvasRef->getShapes().size()) {
				auto& s = canvasRef->getShapes()[index];
				if (contourToggle) {
					s.contourColor = c;
				} else if (fillToggle) {
					s.fillColor = c;
					s.hasFill = true;
				}
			}
		}
	}
	
	colour2dShapes = c;
}

void SceneGraph::contourToggled(bool& val){
	if (val) {
		fillToggle = false;
		backgroundToggle = false;
		if (!selectedShapeIndices.empty()) {
			const auto& s = canvasRef->getShapes()[selectedShapeIndices.back()];
			hueSlider = s.contourColor.getHue();
			satSlider = s.contourColor.getSaturation();
			briSlider = s.contourColor.getBrightness();
			colour2dShapes = s.contourColor;
		}
	} else {
		if (!fillToggle && !backgroundToggle) {
			backgroundToggle = true;
		}
	}
}

void SceneGraph::fillToggled(bool& val) {
	if (val) {
		contourToggle = false;
		backgroundToggle = false;
		if (!selectedShapeIndices.empty()) {
			const auto& s = canvasRef->getShapes()[selectedShapeIndices.back()];
			hueSlider = s.fillColor.getHue();
			satSlider = s.fillColor.getSaturation();
			briSlider = s.fillColor.getBrightness();
			colour2dShapes = s.fillColor;
		}
	} else {
		if (!contourToggle && !backgroundToggle) {
			backgroundToggle = true;
		}
	}
}

void SceneGraph::backgroundToggled(bool& val) {
	if (val) {
		contourToggle = false;
		fillToggle = false;
		if (canvasRef) {
			ofColor bgColor = canvasRef->getBackgroundColor();
			colour2dShapes = bgColor;
			hueSlider = bgColor.getHue();
			satSlider = bgColor.getSaturation();
			briSlider = bgColor.getBrightness();
		}
	} else {
		if (!contourToggle && !fillToggle) {
			contourToggle = true;
		}
	}
}
void SceneGraph::setPanelArea(const ofRectangle& area) {
	panelArea = area;
	x = area.x;
	y = area.y;
	width = area.width;
	height = area.height;

	gui.setPosition(x + panelPadding, y + panelPadding);
	gui.setDefaultWidth(width - 2 * panelPadding);
}

void SceneGraph::mousePressed(int mx, int my, int button) {
	int listStartY = listsStartHeight - panelPadding;
	int rowHeight = 15;
	float panelWidth = 175;

	int numShapes = canvasRef->getShapes().size();
	if (mx >= x + panelPadding && mx <= x + panelPadding + panelWidth &&
		my >= listStartY && my <= listStartY + rowHeight * numShapes)
	{
		int clickedIndex = (my - listStartY) / rowHeight;
		auto it = std::find(selectedShapeIndices.begin(), selectedShapeIndices.end(), clickedIndex);
		if (it != selectedShapeIndices.end()) {
			selectedShapeIndices.erase(it);
		} else {
			if (ofGetKeyPressed(OF_KEY_COMMAND) || ofGetKeyPressed(OF_KEY_CONTROL)) {
				selectedShapeIndices.push_back(clickedIndex);
			} else {
				selectedShapeIndices.clear();
				selectedShapeIndices.push_back(clickedIndex);
			}
		}
		if (!selectedShapeIndices.empty()) {
			const auto& s = canvasRef->getShapes()[selectedShapeIndices.back()];
			thicknessSlider = s.thickness;
			contourToggle = true;
			fillToggle = false;
			//colorSlider = s.contourColor;
			hueSlider = s.contourColor.getHue();
			satSlider = s.contourColor.getSaturation();
			briSlider = s.contourColor.getBrightness();
		}
	}
	
	float panelX = this->x + panelWidth + panelPadding;
	float itemHeight = 15;

	for (int i = 0; i < canvasRef->models.size(); i++) {
		ofRectangle rect(panelX, listStartY + i * itemHeight, panelWidth, itemHeight);
		if (rect.inside(mx, my)) {
			auto it = std::find(selectedModelIndices.begin(), selectedModelIndices.end(), i);
			if (it != selectedModelIndices.end()) {
				selectedModelIndices.erase(it);
			} else {
				if (ofGetKeyPressed(OF_KEY_COMMAND) || ofGetKeyPressed(OF_KEY_CONTROL)) {
					selectedModelIndices.push_back(i);
				} else {
					selectedModelIndices.clear();
					selectedModelIndices.push_back(i);
				}
			}
			if (!canvasRef->models.empty() && i < canvasRef->models.size()) {
				auto& m = canvasRef->models[i];
				setupNormalMappingVisible(m->getEnableNormalMapping());
			}
			break;
		}
	}
	float primitivesPanelX = x + 2 * panelWidth + panelPadding;
	int numPrimitives = canvasRef->getPrimitives3D().size();

	for (int i = 0; i < numPrimitives; i++) {
		ofRectangle rect(primitivesPanelX, listStartY + i * rowHeight, panelWidth, rowHeight);
		if (rect.inside(mx, my)) {
			auto it = std::find(selectedPrimitiveIndices.begin(), selectedPrimitiveIndices.end(), i);
			if (it != selectedPrimitiveIndices.end()) {
				selectedPrimitiveIndices.erase(it);
			} else {
				if (ofGetKeyPressed(OF_KEY_COMMAND) || ofGetKeyPressed(OF_KEY_CONTROL)) {
					selectedPrimitiveIndices.push_back(i);
				} else {
					selectedPrimitiveIndices.clear();
					selectedPrimitiveIndices.push_back(i);
				}
			}
			break;
		}
	}
}
void SceneGraph::drawShapeList() {
	int listStartY = listsStartHeight - panelPadding;
	int rowHeight = 15;
	int numShapes = canvasRef->getShapes().size();
	float panelWidth = 175;
	
	for(int i = 0; i < numShapes; i++) {
		ofRectangle rect(x + panelPadding, listStartY + i * rowHeight,
						panelWidth, rowHeight);
		if(std::find(selectedShapeIndices.begin(), selectedShapeIndices.end(), i) != selectedShapeIndices.end()) {
			ofSetColor(200, 200, 255);
		} else {
			ofSetColor(180);
		}
		ofDrawRectangle(rect);
		ofSetColor(0);
		string shapeType;
		switch(canvasRef->getShapes()[i].type) {
			case ShapeMode::RECTANGLE: shapeType = "Rectangle"; break;
			case ShapeMode::CIRCLE: shapeType = "Cercle"; break;
			case ShapeMode::LINE: shapeType = "Ligne"; break;
			case ShapeMode::FREEFORM: shapeType = "Libre"; break;
			case ShapeMode::SQUARE: shapeType = "Carre"; break;
			case ShapeMode::POINT: shapeType = "Point"; break;
			case ShapeMode::TRIANGLE: shapeType = "Triangle"; break;
			case ShapeMode::HOUSE: shapeType = "Maison";break;
			case ShapeMode::TREE: shapeType = "Arbre";break;
			case ShapeMode::TARGET: shapeType = "Cible"; break;
			default: shapeType = "Unknown"; break;
		}
		
		ofDrawBitmapString(shapeType + " " + std::to_string(i+1), rect.x + 5, rect.y + rowHeight);
	}
}
void SceneGraph::drawModelList() {
	float panelWidth = 175;
	float panelX = x + panelWidth + panelPadding;
	float panelY = listsStartHeight - panelPadding;
	float itemHeight = 15;

	for(int i = 0; i < canvasRef->models.size(); i++) {
		ofRectangle rect(panelX, panelY + i * itemHeight, panelWidth, itemHeight);
		if(std::find(selectedModelIndices.begin(), selectedModelIndices.end(), i) != selectedModelIndices.end()){
			ofSetColor(200, 200, 255);}
		else{
			ofSetColor(180);}
		ofDrawRectangle(rect);
		ofSetColor(0);
		ofDrawBitmapString("Model " + std::to_string(i+1), rect.x + 5, rect.y + itemHeight);
	}
}
void SceneGraph::drawPrimitivesList() {
	float panelWidth = 175;
	float panelX = x + 2 * panelWidth  + panelPadding;
	float listStartY = listsStartHeight - panelPadding;
	int rowHeight = 15;
	int numPrimitives = canvasRef->getPrimitives3D().size();
	
	for(int i = 0; i < numPrimitives; i++) {
		ofRectangle rect(panelX, listStartY + i * rowHeight, panelWidth, rowHeight);
		if(std::find(selectedPrimitiveIndices.begin(), selectedPrimitiveIndices.end(), i) != selectedPrimitiveIndices.end()) {
			ofSetColor(200, 200, 255);
		} else {
			ofSetColor(180);
		}
		ofDrawRectangle(rect);
		ofSetColor(0);
		ofDrawBitmapString("Primitive " + std::to_string(i+1), rect.x + 5, rect.y + rowHeight);
	}
}

void SceneGraph::ensureSingleToggle() {
	int activeCount = (int)contourToggle + (int)fillToggle + (int)backgroundToggle;
	if (activeCount > 1) {
		contourToggle = false;
		fillToggle = false;
		backgroundToggle = false;
		contourToggle = true;
	} else if (activeCount == 0) {
		contourToggle = true;
	}
}

void SceneGraph::deleteButtonPrimitives3DPressed() {
	auto& primitives3D = canvasRef->getPrimitives3D();
	if (!selectedPrimitiveIndices.empty()) {
		std::sort(selectedPrimitiveIndices.begin(), selectedPrimitiveIndices.end(), std::greater<int>());

		for (int index : selectedPrimitiveIndices) {
			if (index >= 0 && index < primitives3D.size()) {
				primitives3D.erase(primitives3D.begin() + index);
			}
		}
		selectedPrimitiveIndices.clear();
	} else if (!primitives3D.empty()) {
		primitives3D.pop_back();
	}
}

void SceneGraph::translationShapeChanged(float& val) {
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];
			s.position.set(translateShapeXSlider, translateShapeYSlider);
		}
	}
}

void SceneGraph::scaleShapeChanged(float& val) {
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];
			s.scale = scaleShapeSlider;
		}
	}
}

void SceneGraph::rotateShapeRightPressed() {
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];
			s.rotation += 15.0f;
			if (s.rotation >= 360.0f) s.rotation -= 360.0f;
		}
	}
}

void SceneGraph::rotateShapeLeftPressed() {
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];
			s.rotation -= 15.0f;
			if (s.rotation < 0.0f) s.rotation += 360.0f;
		}
	}
}

void SceneGraph::textureInversionPressed() {
	auto& models = canvasRef->getModels();
	for (int i : selectedModelIndices) {
		if (i >= 0 && i < models.size() && models[i]) {
			models[i]->toggleProceduralTexture(Texture::INVERSION);
		} else {
			ofLogError() << "Invalid model index: " << i;
		}
	}
}

void SceneGraph::textureWeierstrassPressed(){
	auto& models = canvasRef->getModels();
	for (int i : selectedModelIndices) {
		if (i >= 0 && i < models.size() && models[i]) {
			models[i]->toggleProceduralTexture(Texture::WEIERSTRASS);
		} else {
			ofLogError() << "Invalid model index: " << i;
		}
	}
}
void SceneGraph::textureNormalMappingPressed(){
	auto& models = canvasRef->getModels();
	for (int i : selectedModelIndices) {
		if (i >= 0 && i < models.size() && models[i]) {
			//models[i]->modelDisplacementDefaults[]
			models[i]->toggleProceduralTexture(Texture::NORMAL_MAPPING);
		} else {
			ofLogError() << "Invalid model index: " << i;
		}
	}
}
void SceneGraph::animateSurfacePressed(bool& val) {
	auto& models = canvasRef->getModels();
	for (int i : selectedModelIndices) {
		if (i >= 0 && i < models.size() && models[i]) {
			models[i]->setAnimateSurface(val);
		} else {
			ofLogError() << "Invalid model index: " << i;
		}
	}
}
void SceneGraph::animateColourPressed(bool& val) {
	auto& models = canvasRef->getModels();
	for (int i : selectedModelIndices) {
		if (i >= 0 && i < models.size() && models[i]) {
			models[i]->setAnimateColour(val);
		} else {
			ofLogError() << "Invalid model index: " << i;
		}
	}
}

