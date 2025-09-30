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

	fillToggle.setup("Zone du remplissage", false);
	fillToggle.addListener(this, &SceneGraph::fillToggled);
	gui.add(&fillToggle);

//	colorSlider.setup("Couleur", ofColor(0,0,0), ofColor(0,0,0), ofColor(255,255,255));
//	static_cast<ofParameter<ofColor>&>(colorSlider.getParameter()).addListener(this, &SceneGraph::colorChanged);
//	gui.add(&colorSlider);
	//colorSlider.setDefaultHeight(9);
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
	modelEditorPanel.add(positionSlider.setup("Position", 100.0f, 0.1f, 200.0f));
	
	//modelEditorPanel.add(color_picker_background.set("background color", ofColor(15, 15, 15), ofColor(0, 0), ofColor(255,255)));
	modelEditorPanel.add(color_picker_ambient.set("ambient color", ofColor(63, 63, 63), ofColor(0, 0), ofColor(255, 255)));
	modelEditorPanel.add(color_picker_diffuse.set("diffuse color", ofColor(174, 223, 134), ofColor(0, 0), ofColor(255, 255)));

	
}
void SceneGraph::draw() {
	ofPushStyle();
	ofSetColor(50, 50, 50);
	ofFill();
	ofDrawRectangle(x, y, width, height);
	//ofPopStyle();
	gui.draw();
	drawShapeList();
	ofPopStyle();
}

void SceneGraph::mousePressed(int mx, int my, int button) {
	ofPushStyle();
	int listStartY = 450 - panelPadding;
	int rowHeight = 15;

	int numShapes = canvasRef->getShapes().size();
	if (mx >= x + panelPadding && mx <= x + width - panelPadding &&
		my >= listStartY && my <= listStartY + rowHeight * numShapes)
	{
		selectedIndex = (my - listStartY) / rowHeight;

		const auto& s = canvasRef->getShapes()[selectedIndex];
		thicknessSlider = s.thickness;
		contourToggle = true;
		fillToggle = false;
		colorSlider = s.contourColor;
		hueSlider = s.contourColor.getHue();
		satSlider = s.contourColor.getSaturation();
		briSlider = s.contourColor.getBrightness();
	}
  
	modelEditorPanel.draw();
	drawModelList();
	ofPopStyle();
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
void SceneGraph::thicknessChanged(float & val) {
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];
			s.thickness = val;
		}
	}
}

void SceneGraph::colorChanged(ofColor & col) {
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];
			if (contourToggle) {
				s.contourColor = col;
			}
			else if (fillToggle) {
				s.fillColor = col;
				s.hasFill = true;
			}
		}
	}

	hueSlider = col.getHue();
	satSlider = col.getSaturation();
	briSlider = col.getBrightness();
}

void SceneGraph::hsbChanged(float & val) {
	ofColor c = ofColor::fromHsb(hueSlider, satSlider, briSlider);
	for (int index : selectedShapeIndices) {
		if (index >= 0 && index < canvasRef->getShapes().size()) {
			auto& s = canvasRef->getShapes()[index];

			if (contourToggle) {
				s.contourColor = c;
			}
			else if (fillToggle) {
				s.fillColor = c;
				s.hasFill = true;
			}
		}
	}
	colour2dShapes = c;
	//colorSlider = c;
}

void SceneGraph::contourToggled(bool& val){
	if (val) {
		fillToggle = false;
		if (!selectedShapeIndices.empty()) {
			const auto& s = canvasRef->getShapes()[selectedShapeIndices.back()];
			//colorSlider = s.contourColor;
			hueSlider = s.contourColor.getHue();
			satSlider = s.contourColor.getSaturation();
			briSlider = s.contourColor.getBrightness();
		}
	}
}

void SceneGraph::fillToggled(bool & val) {
	if (val) {
		contourToggle = false;
		if (!selectedShapeIndices.empty()) {
			const auto& s = canvasRef->getShapes()[selectedShapeIndices.back()];
			//colorSlider = s.fillColor;
			hueSlider = s.fillColor.getHue();
			satSlider = s.fillColor.getSaturation();
			briSlider = s.fillColor.getBrightness();
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
	float panelY = 450;
	float itemHeight = 15;

	for (int i = 0; i < canvasRef->models.size(); i++) {
		ofRectangle rect(panelX, panelY + i * itemHeight, panelWidth, itemHeight);
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
	float panelY = 450;
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


