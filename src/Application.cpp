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
	// A garder pour faire fonctionner les clics avec les primitives 3D
	if(!cameras.empty()) {
		canvas.setActiveCamera(&cameras[activeCameraIndex].cam, canvasArea);
	}

	canvas.mousePressed(x, y, button);
	sceneGraph.mousePressed(x, y, button);
	bottomPanel.mousePressed(x, y, button);
	leftPanel.mousePressed(x, y, button);

	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].viewport.inside(x, y)) {
			activeCameraIndex = i;

			// On set la nouvelle camera active pour pouvoir la position des clics sur le canevas 2D en position du monde 3D
			canvas.setActiveCamera(&cameras[activeCameraIndex].cam, canvasArea);

			updateYawPitchFromCamera();
			return;
		}
	}

	if(button == OF_MOUSE_BUTTON_RIGHT) {
		mouseCaptured = !mouseCaptured;
		if(mouseCaptured) {
			orbitMode = !orbitMode;
			// TODO: Change le curseur pour quelques chose comme une quatre fleches en axe
			// ofHideCursor();
		} else {
			ofShowCursor();
		}
	}
	if (selectionMode && button == OF_MOUSE_BUTTON_LEFT && canvasArea.inside(x, y)) {
		isSelecting = true;
		selectionStart = glm::vec2(x - canvasArea.x, y - canvasArea.y);
		selectionEnd = selectionStart;
		selectionRect.set(selectionStart.x, selectionStart.y, 0, 0);
	}
}

void Application::mouseDragged(int x, int y, int button) {
	canvas.mouseDragged(x, y, button);
	
	if (selectionMode && isSelecting && button == OF_MOUSE_BUTTON_LEFT) {
		selectionEnd = glm::vec2(x - canvasArea.x, y - canvasArea.y);
		
		float x1 = glm::min(selectionStart.x, selectionEnd.x);
		float x2 = glm::max(selectionStart.x, selectionEnd.x);
		float y1 = glm::min(selectionStart.y, selectionEnd.y);
		float y2 = glm::max(selectionStart.y, selectionEnd.y);
		
		selectionRect.set(x1, y1, x2 - x1, y2 - y1);
	}
}

void Application::mouseReleased(int x, int y, int button) {
	canvas.mouseReleased(x, y, button);
	
	if (selectionMode && isSelecting && button == OF_MOUSE_BUTTON_LEFT) {
		isSelecting = false;
		selectionEnd = glm::vec2(x - canvasArea.x, y - canvasArea.y);
		
		float x1 = glm::min(selectionStart.x, selectionEnd.x);
		float x2 = glm::max(selectionStart.x, selectionEnd.x);
		float y1 = glm::min(selectionStart.y, selectionEnd.y);
		float y2 = glm::max(selectionStart.y, selectionEnd.y);
		
		selectionRect.set(x1, y1, x2 - x1, y2 - y1);

		if (selectionRect.getWidth() > 10 && selectionRect.getHeight() > 10) {
			focusToSelection();
		}
		selectionRect.set(0, 0, 0, 0);
	}
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
	// pour mettre le focus sur un partie du canevas
	if (selectionMode && isSelecting && selectionRect.getWidth() > 0 && selectionRect.getHeight() > 0) {
			drawSelectionBox();
		}
	
	
	if(!cameras.empty()) {
		// Avant de commencer le draw call on set la camera active
		canvas.setActiveCamera(&cameras[activeCameraIndex].cam, canvasArea);

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
	float moveSpeed = 100.0f * deltaTime;
	float rotateSpeed = 60.0f * deltaTime;
	
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
	}
	else {
		if (coodinates == SPHERICAL) {
			SphericalMovement data = getSphericalMovementData(cam);
			glm::vec3 newPos = calculateSphericalMovement(data, moveSpeed);
			if (newPos != data.position) {
				cam.setPosition(newPos);
			}
			handleSphericalRadiusChange(cam, moveSpeed);
			applyTangentialOrientation(cam, data);
			ofDrawBitmapStringHighlight("Spherical Mode: " + string(sphericalTangentialMode ? "TANGENTIAL" : "FREE-LOOK"),20, 80);
		}
		else if (coodinates == HYPERBOLIC) {
			glm::vec3 pos = cam.getPosition();
			float r = glm::length(pos);
			float scale = 1000.0f;
			scale = cam.getFarClip();
			glm::vec3 currentLookDir = glm::normalize(cam.getLookAtDir());
			glm::vec3 currentUpDir   = glm::normalize(cam.getUpDir());
			float denom = 1.0f - (r * r) / (scale * scale);
			denom = glm::max(denom, 0.001f);
			float hyperbolicFactor = 4.0f / (denom * denom);
			float effectiveSpeed = moveSpeed * hyperbolicFactor * 0.1f;
			if (!std::isfinite(effectiveSpeed)) {
				ofLogWarning() << "moved out of clip plane with r= " << r;
				effectiveSpeed = 0.0f;
			}
			float normalizedR = r / scale;
			float minFOV = 10.0f;
			float maxFOV = 170.0f;
			float hyperbolicFOV = minFOV + (maxFOV - minFOV) * normalizedR * normalizedR;
			float currentFOV = cam.getFov();
			float newFOV = ofLerp(currentFOV, hyperbolicFOV, 0.2f);
			cam.setFov(newFOV);
			if (ofGetKeyPressed('w')) {
				glm::vec3 moveDir = currentLookDir * effectiveSpeed;
				glm::vec3 newPos = hyperbolicTranslate(pos, moveDir, scale);
				cam.setPosition(newPos);
			}
			if (ofGetKeyPressed('s')) {
				glm::vec3 moveDir = -currentLookDir * effectiveSpeed;
				glm::vec3 newPos = hyperbolicTranslate(pos, moveDir, scale);
				cam.setPosition(newPos);
			}
			if (ofGetKeyPressed('a')) {
				glm::vec3 right = glm::normalize(glm::cross(currentLookDir, currentUpDir));
				glm::vec3 moveDir = -right * effectiveSpeed;
				glm::vec3 newPos = hyperbolicTranslate(pos, moveDir, scale);
				cam.setPosition(newPos);
			}
			if (ofGetKeyPressed('d')) {
				glm::vec3 right = glm::normalize(glm::cross(currentLookDir, currentUpDir));
				glm::vec3 moveDir = right * effectiveSpeed;
				glm::vec3 newPos = hyperbolicTranslate(pos, moveDir, scale);
				cam.setPosition(newPos);
			}
			if (ofGetKeyPressed('e')) {
				glm::vec3 moveDir = currentUpDir * effectiveSpeed;
				glm::vec3 newPos = hyperbolicTranslate(pos, moveDir, scale);
				cam.setPosition(newPos);
			}
			if (ofGetKeyPressed('q')) {
				glm::vec3 moveDir = -currentUpDir * effectiveSpeed;
				glm::vec3 newPos = hyperbolicTranslate(pos, moveDir, scale);
				cam.setPosition(newPos);
			}
			cam.lookAt(cam.getPosition() + currentLookDir, currentUpDir);
			ofLogVerbose() << "[HYPERBOLIC] r=" << r << " FOV=" << newFOV << " speed=" << effectiveSpeed;
		}
		else {
			if (ofGetKeyPressed('w')) cam.dolly(-moveSpeed);
			if (ofGetKeyPressed('s')) cam.dolly(moveSpeed);
			if (ofGetKeyPressed('a')) cam.truck(-moveSpeed);
			if (ofGetKeyPressed('d')) cam.truck(moveSpeed);
			if (ofGetKeyPressed('e')) cam.boom(moveSpeed);
			if (ofGetKeyPressed('q')) cam.boom(-moveSpeed);
			cam.setFov(60.0f);
		}
		if (ofGetKeyPressed('f')) cam.panDeg(rotateSpeed);
		if (ofGetKeyPressed('j')) cam.panDeg(-rotateSpeed);
		if (ofGetKeyPressed('g')) cam.tiltDeg(rotateSpeed);
		if (ofGetKeyPressed('h')) cam.tiltDeg(-rotateSpeed);
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
	if (key == 'c' || key == 'C') {
		coodinates = static_cast<Coordinates>((coodinates + 1) % 3);
		
		switch(coodinates) {
			case EUCLIDEAN:
				ofLog() << "Movement mode: Euclidean";
				break;
			case SPHERICAL:
				ofLog() << "Movement mode: Spherical";
				break;
			case HYPERBOLIC:
				ofLog() << "Movement mode: Hyperbolic";
				break;
		}
	}
	if (key == 'k' || key == 'K') {
		sphericalTangentialMode = !sphericalTangentialMode;
		ofLog() << "Spherical sub-mode: " << (sphericalTangentialMode ? "TANGENTIAL" : "FREE-LOOK");
	}
	if (key == 'p' || key == 'P') {
		if (!cameras.empty()) {
			auto& cam = cameras[activeCameraIndex].cam;
			
			if (!cam.getOrtho()) {
				cam.enableOrtho();
				ofLog() << "Projection: Orthogonal";
			} else {
				cam.disableOrtho();
				ofLog() << "Projection: Perspective";
			}
		}
	}
	if (key == '1') {
			selectionMode = !selectionMode;
			if (selectionMode) {
				ofLog() << "Selection mode activated - click and drag to select area";
			} else {
				ofLog() << "Selection mode deactivated";
			}
		}
}

Application::SphericalMovement Application::getSphericalMovementData(ofCamera& cam) {
	SphericalMovement data;
	data.position = cam.getPosition();
	data.surfaceNormal = glm::normalize(data.position);
	data.lookDir = cam.getLookAtDir();
	data.upDir = cam.getUpDir();
	data.rightDir = glm::cross(data.lookDir, data.upDir);
	return data;
}

glm::vec3 Application::calculateSphericalMovement(const SphericalMovement& data, float moveSpeed) {
	glm::vec3 movement(0, 0, 0);
	
	if (ofGetKeyPressed('w')) movement += data.lookDir * moveSpeed;
	if (ofGetKeyPressed('s')) movement -= data.lookDir * moveSpeed;
	if (ofGetKeyPressed('a')) movement -= data.rightDir * moveSpeed;
	if (ofGetKeyPressed('d')) movement += data.rightDir * moveSpeed;
	
	if (glm::length(movement) > 0.0f) {
		glm::vec3 tangentMovement = movement - data.surfaceNormal * glm::dot(movement, data.surfaceNormal);
		glm::vec3 newPos = data.position + tangentMovement;
		float currentRadius = glm::length(data.position);
		return glm::normalize(newPos) * currentRadius;
	}
	
	return data.position;
}

void Application::applyTangentialOrientation(ofCamera& cam, const SphericalMovement& data) {
	if (!sphericalTangentialMode) return;
	
	glm::vec3 newSurfaceNormal = glm::normalize(cam.getPosition());
	glm::vec3 currentLook = cam.getLookAtDir();
	glm::vec3 tangentLook = currentLook - newSurfaceNormal * glm::dot(currentLook, newSurfaceNormal);
	
	if (glm::length(tangentLook) > 0.001f) {
		tangentLook = glm::normalize(tangentLook);
		cam.lookAt(cam.getPosition() + tangentLook, newSurfaceNormal);
	}
}

void Application::handleSphericalRadiusChange(ofCamera& cam, float moveSpeed) {
	glm::vec3 pos = cam.getPosition();
	float currentRadius = glm::length(pos);
	
	if (ofGetKeyPressed('e')) {
		pos = glm::normalize(pos) * (currentRadius + moveSpeed);
		cam.setPosition(pos);
	}
	if (ofGetKeyPressed('q')) {
		float newRadius = glm::max(currentRadius - moveSpeed, 10.0f);
		pos = glm::normalize(pos) * newRadius;
		cam.setPosition(pos);
	}
}

glm::vec3 Application::hyperbolicTranslate(const glm::vec3& pos, const glm::vec3& dir, float scale) {
	float r = glm::length(pos);
	if (r > scale * 0.95f) {
		return pos;
	}
	float boost = 1.0f / (1.0f - (r*r)/(scale*scale));
	glm::vec3 hyperbolicDir = dir * boost;
	glm::vec3 newPos = pos + hyperbolicDir;
	float newR = glm::length(newPos);
	if (newR >= scale) {
		newPos = glm::normalize(newPos) * scale * 0.95f;
	}
	return newPos;
}

void Application::drawSelectionBox() {
	if (selectionRect.getWidth() <= 0 || selectionRect.getHeight() <= 0) return;
	
	ofPushStyle();
	float canvasAspect = canvasArea.width / canvasArea.height;
	float selWidth = selectionRect.width;
	float selHeight = selectionRect.height;
	float selAspect = selWidth / selHeight;
	ofRectangle proportionalRect = selectionRect;
	
	if (selAspect > canvasAspect) {
		float newHeight = selWidth / canvasAspect;
		proportionalRect.height = newHeight;
		proportionalRect.y = selectionRect.y - (newHeight - selHeight) * 0.5f;
	}
	else {
		float newWidth = selHeight * canvasAspect;
		proportionalRect.width = newWidth;
		proportionalRect.x = selectionRect.x - (newWidth - selWidth) * 0.5f;
	}
	ofNoFill();
	ofSetColor(255, 255, 0);
	ofSetLineWidth(2);

	ofDrawRectangle(proportionalRect.x + canvasArea.x,proportionalRect.y + canvasArea.y,proportionalRect.width,proportionalRect.height);

	ofSetColor(255, 0, 0);
	float crosshairSize = 5.0f;
	float drawX = proportionalRect.x + canvasArea.x;
	float drawY = proportionalRect.y + canvasArea.y;
	float drawWidth = proportionalRect.width;
	float drawHeight = proportionalRect.height;
	
	// Top-left corner
	ofDrawLine(drawX, drawY - crosshairSize,
			   drawX, drawY + crosshairSize);
	ofDrawLine(drawX - crosshairSize, drawY,
			   drawX + crosshairSize, drawY);
	
	// Top-right corner
	ofDrawLine(drawX + drawWidth, drawY - crosshairSize,
			   drawX + drawWidth, drawY + crosshairSize);
	ofDrawLine(drawX + drawWidth - crosshairSize, drawY,
			   drawX + drawWidth + crosshairSize, drawY);
	
	// Bottom-left corner
	ofDrawLine(drawX, drawY + drawHeight - crosshairSize,
			   drawX, drawY + drawHeight + crosshairSize);
	ofDrawLine(drawX - crosshairSize, drawY + drawHeight,
			   drawX + crosshairSize, drawY + drawHeight);
	
	// Bottom-right corner
	ofDrawLine(drawX + drawWidth, drawY + drawHeight - crosshairSize,
			   drawX + drawWidth, drawY + drawHeight + crosshairSize);
	ofDrawLine(drawX + drawWidth - crosshairSize, drawY + drawHeight,
			   drawX + drawWidth + crosshairSize, drawY + drawHeight);
	
	ofSetColor(255, 255, 255);
	string dimensions = ofToString((int)proportionalRect.width) + " x " + ofToString((int)proportionalRect.height);
	ofDrawBitmapString(dimensions,
					  drawX + drawWidth * 0.5f - dimensions.length() * 3,
					  drawY + drawHeight * 0.5f);
	
	string aspectInfo = "Aspect: " + ofToString(canvasAspect, 2);
	//ofDrawBitmapString(aspectInfo,drawX + drawWidth * 0.5f - aspectInfo.length() * 3,drawY + drawHeight * 0.5f + 15);
	ofPopStyle();
}

void Application::focusToSelection() {
	if (cameras.empty()) return;
	
	auto& cam = cameras[activeCameraIndex].cam;
	glm::vec2 selectionCenter = glm::vec2(selectionRect.x + selectionRect.width * 0.5f,selectionRect.y + selectionRect.height * 0.5f);
	glm::vec2 canvasCenter = glm::vec2(canvasArea.width * 0.5f, canvasArea.height * 0.5f);

	glm::vec2 pixelOffset;
	pixelOffset.x = selectionCenter.x - canvasCenter.x;
	pixelOffset.y = selectionCenter.y - canvasCenter.y;

	glm::vec2 normalizedOffset;
	normalizedOffset.x = pixelOffset.x / canvasCenter.x;
	normalizedOffset.y = -pixelOffset.y / canvasCenter.y;
	
	glm::vec3 camPos = cam.getPosition();
	float currentDistance = glm::length(camPos);
	
	cam.truck(normalizedOffset.x * currentDistance);
	cam.boom(normalizedOffset.y * currentDistance);
	
	float widthRatio = canvasArea.width / selectionRect.width;
	float heightRatio = canvasArea.height / selectionRect.height;
	float zoomFactor = glm::min(widthRatio, heightRatio);
	
	float dollyDistance = currentDistance * (1.0f - (1.0f / zoomFactor));
	dollyDistance = glm::clamp(dollyDistance, 10.0f, currentDistance * 0.8f);
	
	cam.dolly(-dollyDistance);
	
	ofLog() << "Centered and dollied in by: " << dollyDistance;
}
