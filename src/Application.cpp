#include "Application.h"

void Application::setup() {
	ofSetWindowShape(1024, 768);

	float sidebarWidth = 224.0f;
	float bottomPanelHeight = 100.0f;

	leftPanelArea.set(0, 0, sidebarWidth, ofGetHeight() - bottomPanelHeight);
	bottomPanelArea.set(0, ofGetHeight() - bottomPanelHeight, ofGetWidth(), bottomPanelHeight);
	canvasArea.set(sidebarWidth, 0, ofGetWidth() - sidebarWidth, ofGetHeight() - bottomPanelHeight);
	sceneGraphArea.set(ofGetWidth() - sidebarWidth, 0, sidebarWidth, ofGetHeight());

	canvas.setup(canvasArea,&toolbar);
	canvas.setDrawingArea(canvasArea);

	toolbar.setup(&canvas);
	sceneGraph.setup(&canvas, sceneGraphArea);
	bottomPanel.setup(&canvas, bottomPanelArea);
	leftPanel.setup(&canvas, leftPanelArea);
}

void Application::windowResized(int w, int h) {
	float sidebarWidth = 224.0f;
	float bottomPanelHeight = 100.0f;

	leftPanelArea.set(0, 0, sidebarWidth, h);
	bottomPanelArea.set(0, h - bottomPanelHeight, w, bottomPanelHeight);
	canvasArea.set(sidebarWidth, 0, w - sidebarWidth, h - bottomPanelHeight);
	sceneGraphArea.set(w - sidebarWidth, 0, sidebarWidth, h);

	canvas.setDrawingArea(canvasArea);
	sceneGraph.setPanelArea(sceneGraphArea);
	bottomPanel.setPanelArea(bottomPanelArea);
	leftPanel.setPanelArea(leftPanelArea);
}

void Application::update() {
	canvas.update();
}

void Application::draw() {
	canvas.draw();
	sceneGraph.draw();
	bottomPanel.draw();
	leftPanel.draw();
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