#include "Application.h"

void Application::setup() {
	ofSetWindowShape(1700, 1024);
	//ofEnableDepthTest();
	//glEnable(GL_DEPTH_TEST);
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
	updateActiveCamera();
}

void Application::mousePressed(int x, int y, int button) {
	canvas.mousePressed(x, y, button);
	sceneGraph.mousePressed(x, y, button);
	bottomPanel.mousePressed(x, y, button);
	leftPanel.mousePressed(x, y, button);

	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].viewport.inside(x, y)) {
			activeCameraIndex = i;
			updateYawPitchFromCamera();
			return;
		}
	}

	if(button == OF_MOUSE_BUTTON_RIGHT) {
		mouseCaptured = !mouseCaptured;
		if(mouseCaptured) {
			orbitMode = !orbitMode; // Toggle between control schemes
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
		//DEBUG////////////////////////////////////////////////////
		drawDebugAxes(300.0f, 4.0f);
		//DEBUG////////////////////////////////////////////////////
		cameras[activeCameraIndex].cam.end();
		ofPopView();
		
		// Pour les autres cameras
		for(int i = 0; i < cameras.size(); i++) {
			ofPushView();
			ofViewport(cameras[i].viewport);
			cameras[i].cam.begin();
			canvas.draw3d();
			//DEBUG////////////////////////////////////////////////////
			drawDebugAxes(300.0f, 4.0f);
			//DEBUG////////////////////////////////////////////////////
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
			ofSetColor(0,0,0);
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
		
		cv.cam.setFov(60.0f);
		
		cv.cam.setNearClip(10.0f);
		cv.cam.setFarClip(10000.0f);

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
	updateYawPitchFromCamera();
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

void Application::updateActiveCamera() {
	if (cameras.empty()) return;
	
	float deltaTime = ofGetLastFrameTime();
	float moveSpeed = 200.0f * deltaTime;
	float rotateSpeed = 30.0f * deltaTime;
	
	auto& cam = cameras[activeCameraIndex].cam;

	if (orbitMode && mouseCaptured) {
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
		glm::vec3 up = glm::normalize(glm::cross(right, front));

		glm::vec3 pos = cam.getPosition();

		if (ofGetKeyPressed('w')) pos += front * moveSpeed;
		if (ofGetKeyPressed('s')) pos -= front * moveSpeed;
		if (ofGetKeyPressed('a')) pos -= right * moveSpeed;
		if (ofGetKeyPressed('d')) pos += right * moveSpeed;
		if (ofGetKeyPressed('e')) pos += up * moveSpeed;
		if (ofGetKeyPressed('q')) pos -= up * moveSpeed;

		cam.setPosition(pos);
		cam.lookAt(pos + front, up);
	} else {
		if (ofGetKeyPressed('w')) cam.dolly(-moveSpeed);  // Forward
		if (ofGetKeyPressed('s')) cam.dolly(moveSpeed);   // Backward
		if (ofGetKeyPressed('a')) cam.truck(-moveSpeed);  // Left
		if (ofGetKeyPressed('d')) cam.truck(moveSpeed);   // Right
		if (ofGetKeyPressed('e')) cam.boom(moveSpeed);    // Up
		if (ofGetKeyPressed('q')) cam.boom(-moveSpeed);   // Down
		
		if (ofGetKeyPressed('f')) cam.panDeg(rotateSpeed);   // Rotate left
		if (ofGetKeyPressed('j')) cam.panDeg(-rotateSpeed);  // Rotate right
		
		if (ofGetKeyPressed('g')) cam.tiltDeg(rotateSpeed);   // Tilt up
		if (ofGetKeyPressed('h')) cam.tiltDeg(-rotateSpeed);  // Tilt down
		
	}
}

void Application::resetCamerasToSphere() {
	float sphereRadius = 1000.0f;
	
	for (int i = 0; i < cameras.size(); i++) {
		float horizontalAngle = i * (TWO_PI / cameras.size());
		float verticalAngle = PI / 6.0f;
		
		float x = sphereRadius * cos(horizontalAngle) * cos(verticalAngle);
		float y = sphereRadius * sin(verticalAngle);
		float z = sphereRadius * sin(horizontalAngle) * cos(verticalAngle);
		
		cameras[i].cam.setPosition(x, y, z);
		cameras[i].cam.lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}
	yaw = 0.0f;
	pitch = 0.0f;
	
	ofLog() << "Cameras reset to spherical arrangement :)";
}
void Application::updateYawPitchFromCamera() {
	if (cameras.empty() || !orbitMode) return;
	
	auto& cam = cameras[activeCameraIndex].cam;
	glm::vec3 front = glm::normalize(-cam.getSideDir());
	
	pitch = glm::degrees(asinf(front.y));
	yaw = glm::degrees(atan2f(front.z, front.x));
}
void Application::drawDebugAxes(float length, float thickness) {
	ofSetLineWidth(thickness);
	
	// X-axis (Red)
	ofSetColor(255, 0, 0);
	ofDrawLine(0, 0, 0, length, 0, 0);
	ofDrawBitmapString("X", length + 10, 0, 0);
	
	// Y-axis (Green)
	ofSetColor(0, 255, 0);
	ofDrawLine(0, 0, 0, 0, length, 0);
	ofDrawBitmapString("Y", 0, length + 10, 0);
	
	// Z-axis (Blue)
	ofSetColor(0, 0, 255);
	ofDrawLine(0, 0, 0, 0, 0, length);
	ofDrawBitmapString("Z", 0, 0, length + 10);
	
	// Origin sphere
	ofSetColor(255, 255, 255);
	ofDrawSphere(0, 0, 0, 5.0f);
	
	ofSetLineWidth(1);
}

void Application::keyPressed(int key) {
	if (key == 'r' || key == 'R') {
		resetCamerasToSphere();
	}
}
