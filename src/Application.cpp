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

	if (!cameras.empty()) {
		float moveSpeed = 10.0f;
		auto &cam = cameras[activeCameraIndex].cam;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front   = glm::normalize(front);

		glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0,1,0)));
		glm::vec3 up    = glm::normalize(glm::cross(right, front));

		glm::vec3 pos = cam.getPosition();

		if (ofGetKeyPressed('w')) pos += front * moveSpeed;
		if (ofGetKeyPressed('s')) pos -= front * moveSpeed;
		if (ofGetKeyPressed('a')) pos -= right * moveSpeed;
		if (ofGetKeyPressed('d')) pos += right * moveSpeed;

		cam.setPosition(pos);

		// Change l'orientation de la cam, selon le point ou on regarde
		cam.lookAt(pos + front, up);
	}
}

void Application::mousePressed(int x, int y, int button) {
	canvas.mousePressed(x, y, button);
	sceneGraph.mousePressed(x, y, button);
	bottomPanel.mousePressed(x, y, button);
	leftPanel.mousePressed(x, y, button);

	//bool commandHeld = ofGetKeyPressed(OF_KEY_COMMAND); //OF_KEY_CONTROL
	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].viewport.inside(x, y)) {
			activeCameraIndex = i;
			return;
		}
	}

	if(button == OF_MOUSE_BUTTON_RIGHT) {
		mouseCaptured = !mouseCaptured;
		if(mouseCaptured) {
			// TODO: Change le curseur pour quelques chose comme une quatre fleches en axe
			// ofHideCursor();
		} else {
			ofShowCursor();
		}
	}
}

void Application::mouseDragged(int x, int y, int button) {
	canvas.mouseDragged(x, y, button);
}

void Application::mouseReleased(int x, int y, int button) {
	canvas.mouseReleased(x, y, button);
}

// Pour la camera
void Application::mouseMoved(int x, int y) {
	static int lastX = x;
	static int lastY = y;

	if(mouseCaptured && !cameras.empty()) {
		float dx = x - lastX;
		float dy = y - lastY;

		yaw   += dx * mouseSensitivity;
		pitch -= dy * mouseSensitivity;

		pitch = ofClamp(pitch, -89.0f, 89.0f);
	}

	lastX = x;
	lastY = y;
}

void Application::draw() {
	canvas.draw2d();
	
	leftPanel.draw();
	sceneGraph.draw();
	//bottomPanel.draw();
	toolbar.draw();
	
	
	if(!cameras.empty()) {
		// Pour la camera sur le canevas
		ofPushView();
		ofViewport(canvasArea);
		cameras[activeCameraIndex].cam.begin();

		canvas.draw3d();
		cameras[activeCameraIndex].cam.end();
		ofPopView();
		
		// Pour les autres cameras
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
