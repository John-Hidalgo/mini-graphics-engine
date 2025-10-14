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
	
	deleteButton3DModel.setup("Effacez");
	deleteButton3DModel.addListener(this,&SceneGraph::deleteButton3DModelPressed);
	modelEditorPanel.add(&deleteButton3DModel);


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

        // Logging for debugging
        ofLog() << "SelectionRect: x=" << selectionRect.x
                << " y=" << selectionRect.y
                << " w=" << selectionRect.width
                << " h=" << selectionRect.height;

        ofLog() << "Shape " << i << " Bounds: x=" << shapeBounds.x
                << " y=" << shapeBounds.y
                << " w=" << shapeBounds.width
                << " h=" << shapeBounds.height;

        bool intersecting = selectionRect.intersects(shapeBounds);
        ofLog() << "→ Intersecting: " << (intersecting ? "YES" : "NO");

        if (intersecting) {
            selectedShapeIndices.push_back(i);
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
	int listStartY = 450 - panelPadding;
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
	int listStartY = 450 - panelPadding;
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
		ofDrawBitmapString("Shape " + std::to_string(i+1), rect.x + 5, rect.y + rowHeight);
	}
}
void SceneGraph::drawModelList() {
	float panelWidth = 175;
	float panelX = x + panelWidth + panelPadding;
	float panelY = 450 - panelPadding;
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
	float listStartY = 450 - panelPadding;
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