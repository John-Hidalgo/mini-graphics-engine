#include "Application.h"

void Application::setup() {
	ofSetWindowShape(1024, 768);

	float sidebarWidth = 224.0f;
	float bottomPanelHeight = 100.0f;

	leftPanel.set(0, 0, sidebarWidth, ofGetHeight());
	bottomPanel.set(0, ofGetHeight() - bottomPanelHeight, ofGetWidth(), bottomPanelHeight);
	canvasArea.set(sidebarWidth, 0, ofGetWidth() - sidebarWidth, ofGetHeight() - bottomPanelHeight);
	sceneGraphArea.set(ofGetWidth() - sidebarWidth, 0, sidebarWidth, ofGetHeight());

	canvas.setup(canvasArea,&toolbar);
	canvas.setDrawingArea(canvasArea);

	toolbar.setup(&canvas);
	sceneGraph.setup(&canvas, sceneGraphArea);
}

void Application::windowResized(int w, int h) {
	float sidebarWidth = 224.0f;
	float bottomPanelHeight = 100.0f;

	leftPanel.set(0, 0, sidebarWidth, h);
	bottomPanel.set(0, h - bottomPanelHeight, w, bottomPanelHeight);
	canvasArea.set(sidebarWidth, 0, w - sidebarWidth, h - bottomPanelHeight);
	sceneGraphArea.set(w - sidebarWidth, 0, sidebarWidth, h);

	canvas.setDrawingArea(canvasArea);
	sceneGraph.setPanelArea(sceneGraphArea);
}

void Application::update() {
	canvas.update();
}

void Application::draw() {
	ofPushStyle();
	ofSetColor(50, 50, 50);
	ofDrawRectangle(leftPanel);
	ofDrawRectangle(bottomPanel);

	// Borders in black
	ofSetColor(0, 0, 0); // Noir
	ofNoFill();
	ofSetLineWidth(2);
	ofDrawRectangle(leftPanel);
	ofDrawRectangle(bottomPanel);
	ofDrawRectangle(sceneGraphArea);
	ofFill();
	ofPopStyle();


	// Réinitialiser la couleur pour les autres dessins
	ofSetColor(255, 255, 255);

	canvas.draw();
	sceneGraph.draw();
	toolbar.draw();
}

void Application::mousePressed(int x, int y, int button) {
	canvas.mousePressed(x, y, button);
	sceneGraph.mousePressed(x, y, button);
}

void Application::mouseDragged(int x, int y, int button) {
	canvas.mouseDragged(x, y, button);
}

void Application::mouseReleased(int x, int y, int button) {
	canvas.mouseReleased(x, y, button);
}