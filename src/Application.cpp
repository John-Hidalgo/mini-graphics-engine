#include "Application.h"

void Application::setup() {
	ofSetWindowShape(1700, 1024);
	
	leftPanelArea.set(0, 0, leftPanelWidth, ofGetHeight() - bottomPanelHeight);
	leftPanel.setup(&canvas, leftPanelArea);
	
	canvasArea.set(leftPanelWidth, 0, ofGetWidth() - leftPanelWidth - rightPanelWidth,
				   ofGetHeight() - bottomPanelHeight);
	canvas.setup(canvasArea, &toolbar, &sceneGraph);
	canvas.setDrawingArea(canvasArea);
	
	sceneGraphArea.set(ofGetWidth() - rightPanelWidth, 0, rightPanelWidth, ofGetHeight());
	sceneGraph.setup(&canvas, sceneGraphArea);

	bottomPanelArea.set(0, ofGetHeight() - bottomPanelHeight, ofGetWidth(), bottomPanelHeight);
	bottomPanel.setup(&canvas, bottomPanelArea);

	toolbar.setup(&canvas);
	setupCameras();
	updateCameraViewports(ofGetWidth(), ofGetHeight());
}
void Application::windowResized(int w, int h) {
	leftPanelArea.set(0, 0, leftPanelWidth, h - bottomPanelHeight);
	leftPanel.setup(&canvas, leftPanelArea);
	
	canvasArea.set(leftPanelWidth, 0,
				   w - leftPanelWidth - rightPanelWidth,
				   h - bottomPanelHeight);
	canvas.setup(canvasArea, &toolbar, &sceneGraph);
	canvas.setDrawingArea(canvasArea);
	
	sceneGraphArea.set(w - rightPanelWidth, 0, rightPanelWidth, h);
	sceneGraph.setup(&canvas, sceneGraphArea);

	bottomPanelArea.set(0, h - bottomPanelHeight, w, bottomPanelHeight);
	bottomPanel.setup(&canvas, bottomPanelArea);
	updateCameraViewports(w, h);
}

void Application::update() {
	canvas.update();
}

//void Application::draw() {
//	leftPanel.draw();
//	canvas.draw();
//	sceneGraph.draw();
//	bottomPanel.draw();
//	toolbar.draw();
//}

void Application::mousePressed(int x, int y, int button) {
	// A garder pour faire fonctionner les clics avec les primitives 3D
	if(!cameras.empty()) {
		canvas.setActiveCamera(&cameras[activeCameraIndex].cam, canvasArea);
	}

	canvas.mousePressed(x, y, button);
	sceneGraph.mousePressed(x, y, button);
	bottomPanel.mousePressed(x, y, button);
	leftPanel.mousePressed(x, y, button);

	//bool commandHeld = ofGetKeyPressed(OF_KEY_COMMAND); //OF_KEY_CONTROL
	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].viewport.inside(x, y)) {
			activeCameraIndex = i;

			// On set la nouvelle camera active pour pouvoir la position des clics sur le canevas 2D en position du monde 3D
			canvas.setActiveCamera(&cameras[activeCameraIndex].cam, canvasArea);

			return;
		}
	}
}

void Application::mouseDragged(int x, int y, int button) {
	canvas.mouseDragged(x, y, button);
}

void Application::mouseReleased(int x, int y, int button) {
	canvas.mouseReleased(x, y, button);
}

void Application::draw() {
	canvas.draw2d();
	
	leftPanel.draw();
	sceneGraph.draw();
	//bottomPanel.draw();
	toolbar.draw();
	
	
	if(!cameras.empty()) {
		// Avant de commencer le draw call on set la camera active
		canvas.setActiveCamera(&cameras[activeCameraIndex].cam, canvasArea);

		//pour la camera sur le caneva
		ofPushView();
		ofViewport(canvasArea);
		cameras[activeCameraIndex].cam.begin();
		canvas.draw3d();
		cameras[activeCameraIndex].cam.end();
		ofPopView();
		
		// pour les autres
		for(int i = 0; i < cameras.size(); i++) {
			ofPushView();
			ofViewport(cameras[i].viewport);
			cameras[i].cam.begin();
			canvas.draw3d();
			cameras[i].cam.end();
			ofPopView();
			
			ofNoFill();
			if(i == activeCameraIndex) {
				ofSetColor(255, 255, 0);
			} else {
				ofSetColor(255);
			}
			ofDrawRectangle(cameras[i].viewport);
			ofFill();
			ofSetColor(255);
			ofDrawBitmapString("Cam " + ofToString(i+1),
							 cameras[i].viewport.x + 5,
							 cameras[i].viewport.y + 15);
		}
	}
}

void Application::setupCameras(){
	int numCameras = 4;
	int bottomMargin = 0;
	float sphereRadius = 1000.0f;
	
	for (int i = 0; i < numCameras; i++) {
		CameraView cv;
		float horizontalAngle = i * (TWO_PI / numCameras);
		float verticalAngle = PI / 6.0f;
		
		float x = sphereRadius * cos(horizontalAngle) * cos(verticalAngle);
		float y = sphereRadius * sin(verticalAngle);
		float z = sphereRadius * sin(horizontalAngle) * cos(verticalAngle);
		cv.cam.setPosition(x, y, z);
		
		cv.cam.lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		
		cv.cam.setFov(120.0f);
		
		cv.cam.setNearClip(10.0f);
		cv.cam.setFarClip(2000.0f);

		cv.viewport = ofRectangle(
			10 + i * (previewWidth + 10),
			ofGetHeight() - previewHeight - bottomMargin,
			previewWidth,
			previewHeight
		);

		cameras.push_back(cv);
		//ofLog() << "Camera " << i << " position: "<< "x=" << x << ", y=" << y << ", z=" << z;
	}

	activeCameraIndex = 0;
}

void Application::updateCameraViewports(int w, int h) {
	int bottomMargin = 10;
	
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i].viewport = ofRectangle(
			10 + i * (previewWidth + 10),
			h - previewHeight - bottomMargin,
			previewWidth,
			previewHeight
		);
	}
}
