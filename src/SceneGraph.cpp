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

	gui.setBackgroundColor(ofColor(50, 50, 50));
	gui.setBorderColor(ofColor(100, 100, 100));
	gui.setTextColor(ofColor(255, 255, 255));
	gui.setHeaderBackgroundColor(ofColor(50, 50, 50));
	gui.setDefaultWidth(width - 2 * panelPadding);

	thicknessSlider.setup("Epaisseur", 2.0f, 1.0f, 20.0f);
	thicknessSlider.addListener(this, &SceneGraph::thicknessChanged);
	gui.add(&thicknessSlider);
	
	contourToggle.setup("Contour", true);
	contourToggle.addListener(this, &SceneGraph::contourToggled);
	gui.add(&contourToggle);

	fillToggle.setup("Zone du remplissage", false);
	fillToggle.addListener(this, &SceneGraph::fillToggled);
	gui.add(&fillToggle);

	colorSlider.setup("Couleur", ofColor(0,0,0), ofColor(0,0,0), ofColor(255,255,255));
	static_cast<ofParameter<ofColor>&>(colorSlider.getParameter()).addListener(this, &SceneGraph::colorChanged);
	gui.add(&colorSlider);
	
	gui.add(hueSlider.setup("Hue", 128, 0, 255));
	gui.add(satSlider.setup("Saturation", 200, 0, 255));
	gui.add(briSlider.setup("Brightness", 200, 0, 255));
	hueSlider.addListener(this, &SceneGraph::hsbChanged);
	satSlider.addListener(this, &SceneGraph::hsbChanged);
	briSlider.addListener(this, &SceneGraph::hsbChanged);


	deleteButton.setup("Effacez");
	deleteButton.addListener(this, &SceneGraph::deleteButtonPressed);
	gui.add(&deleteButton);
}
void SceneGraph::drawShapeList() {
	const auto& shapes = canvasRef->getShapes();
	int yOffset = 450;
	int i = 0;

	for (auto& s : shapes) {
		if (i == selectedIndex) {
			ofSetColor(100, 100, 255);
			ofDrawRectangle(x + panelPadding, y + yOffset - 12, width - 2*panelPadding, 15);
		}

		ofSetColor(255);
		std::string typeName;
		switch (s.type) {
			case ShapeMode::RECTANGLE: typeName = "Rectangle"; break;
			case ShapeMode::CIRCLE:    typeName = "Cercle"; break;
			case ShapeMode::LINE:      typeName = "Ligne"; break;
			case ShapeMode::FREEFORM:  typeName = "Libre"; break;
			default: typeName = "Unknown"; break;
		}

		std::string label = ofToString(i) + ": " + typeName;
		ofDrawBitmapString(label, x + panelPadding, y + yOffset);
		yOffset += 15;
		i++;
	}
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
	ofPopStyle();
}
void SceneGraph::deleteButtonPressed() {
	auto& shapes = canvasRef->getShapes();
	if (!shapes.empty()) {
		shapes.pop_back();
	}
}
void SceneGraph::thicknessChanged(float & val) {
	if (selectedIndex >= 0) {
		auto& s = canvasRef->getShapes()[selectedIndex];
		s.thickness = val;
	}
}
void SceneGraph::colorChanged(ofColor & col) {
	if (selectedIndex >= 0) {
		auto& s = canvasRef->getShapes()[selectedIndex];

		if (contourToggle) {
			s.contourColor = col;
		}
		else if (fillToggle) {
			s.fillColor = col;
			s.hasFill = true;
		}
	}

	hueSlider = col.getHue();
	satSlider = col.getSaturation();
	briSlider = col.getBrightness();
}
void SceneGraph::hsbChanged(float & val) {
	ofColor c = ofColor::fromHsb(hueSlider, satSlider, briSlider);

	if (selectedIndex >= 0) {
		auto& s = canvasRef->getShapes()[selectedIndex];

		if (contourToggle) {
			s.contourColor = c;
		}
		else if (fillToggle) {
			s.fillColor = c;
			s.hasFill = true;
		}
	}
	colorSlider = c;
}

void SceneGraph::contourToggled(bool& val){
	if (val) {
		fillToggle = false;
		if (selectedIndex >= 0) {
			const auto& s = canvasRef->getShapes()[selectedIndex];
			colorSlider = s.contourColor;
			hueSlider = s.contourColor.getHue();
			satSlider = s.contourColor.getSaturation();
			briSlider = s.contourColor.getBrightness();
		}
	}
	
}
void SceneGraph::fillToggled(bool & val) {
	if (val) {
		contourToggle = false;
		if (selectedIndex >= 0) {
			const auto& s = canvasRef->getShapes()[selectedIndex];
			colorSlider = s.fillColor;
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
