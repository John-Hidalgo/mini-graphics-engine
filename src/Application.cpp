#include "Application.h"

void Application::setup() {
	ofSetWindowShape(1700, 1024);
	
	leftPanelArea.set(0, 0, leftPanelWidth, ofGetHeight() - bottomPanelHeight);
	leftPanel.setup(&canvas, leftPanelArea);
	
	canvasArea.set(leftPanelWidth, 0, ofGetWidth() - leftPanelWidth - rightPanelWidth, ofGetHeight() - bottomPanelHeight);
	canvas.setup(canvasArea, &toolbar, &sceneGraph);
	canvas.setDrawingArea(canvasArea);
	
	sceneGraphArea.set(ofGetWidth() - rightPanelWidth, 0, rightPanelWidth, ofGetHeight());
	sceneGraph.setup(&canvas, sceneGraphArea);

	bottomPanelArea.set(0, ofGetHeight() - bottomPanelHeight, ofGetWidth(), bottomPanelHeight);
	bottomPanel.setup(&canvas, bottomPanelArea);

	toolbar.setup(&canvas);
}
void Application::windowResized(int w, int h) {
	leftPanelArea.set(0, 0, leftPanelWidth, w - bottomPanelHeight);
	leftPanel.setup(&canvas, leftPanelArea);
	
	canvasArea.set(leftPanelWidth, 0, w - leftPanelWidth - rightPanelWidth, w - bottomPanelHeight);
	canvas.setup(canvasArea, &toolbar, &sceneGraph);
	canvas.setDrawingArea(canvasArea);
	
	sceneGraphArea.set(w - rightPanelWidth, 0, rightPanelWidth, w);
	sceneGraph.setup(&canvas, sceneGraphArea);

	bottomPanelArea.set(0, h - bottomPanelHeight, w, bottomPanelHeight);
	bottomPanel.setup(&canvas, bottomPanelArea);
}

void Application::update() {
	canvas.update();
}

void Application::draw() {
	leftPanel.draw();
	canvas.draw();
	sceneGraph.draw();
	bottomPanel.draw();
	toolbar.draw();
}

void Application::mousePressed(int x, int y, int button) {
	canvas.mousePressed(x, y, button);
	sceneGraph.mousePressed(x, y, button);
	bottomPanel.mousePressed(x, y, button);
	leftPanel.mousePressed(x, y, button);
}

void Application::mouseDragged(int x, int y, int button) {
	canvas.mouseDragged(x, y, button);
}

void Application::mouseReleased(int x, int y, int button) {
	canvas.mouseReleased(x, y, button);
}
