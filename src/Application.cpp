#include "Application.h"


void Application::setup() {
	ofSetWindowShape(1700, 1024);

	//float sidebarWidth = 224.0f;

	canvasArea.set(0, 0, ofGetWidth() - sidebarWidth, ofGetHeight());
	sceneGraphArea.set(ofGetWidth() - sidebarWidth, 0, sidebarWidth, ofGetHeight());

	canvas.setup(canvasArea,&toolbar,&sceneGraph);
	canvas.setDrawingArea(canvasArea);

	toolbar.setup(&canvas);
	sceneGraph.setup(&canvas, sceneGraphArea);
}

void Application::windowResized(int w, int h) {
	//float sidebarWidth = 224.0f;

	canvasArea.set(0, 0, w - sidebarWidth, h);
	sceneGraphArea.set(w - sidebarWidth, 0, sidebarWidth, h);

	canvas.setDrawingArea(canvasArea);
	sceneGraph.setPanelArea(sceneGraphArea);
}

void Application::update() {
	canvas.update();
}

void Application::draw() {
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
