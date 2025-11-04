#include "DeferredRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

DeferredRenderer::DeferredRenderer(float leftPanel, float RightPanel, float BottomPanel)
	: leftPanelWidth(leftPanel), rightPanelWidth(RightPanel), bottomPanelHeight(BottomPanel) {}

void DeferredRenderer::setup() {
	ofDisableArbTex();
	deferred.setup();
	cube = Mesh::Cube();
	setupFixedCamera();
	setupEasyCamera();
}

void DeferredRenderer::setupFixedCamera() {
	fixedProj = glm::perspective(glm::radians(45.0f), 1500.0f / 1000.0f, 0.1f, 100.0f);
	fixedView = glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void DeferredRenderer::setupEasyCamera() {
	easyCam.setDistance(15.0f);
	easyCam.setNearClip(0.1f);
	easyCam.setFarClip(100.0f);
	easyCam.setPosition(0, 10, 10);
	easyCam.lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void DeferredRenderer::update() {
	angle += ofGetLastFrameTime();
}

void DeferredRenderer::draw() {
	ofDisableAlphaBlending();
	int windowW = ofGetWidth();
	int windowH = ofGetHeight();

	float scaleW = 0.56f;
	float scaleH = 0.839f;

	int renderW = int(windowW * scaleW);
	int renderH = int(windowH * scaleH);

	int viewportX = leftPanelWidth;
	int viewportY = windowH - renderH;

	std::vector<Mesh> cubes(9, cube);
	std::vector<glm::mat4> models(9);
	int floorSize = 20;
	float cubeSize = 1.0f;
	for (int x = -floorSize; x <= floorSize; ++x) {
		for (int z = -floorSize; z <= floorSize; ++z) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(x * cubeSize, -cubeSize / 2.0f, z * cubeSize));
			models.push_back(model);
			cubes.push_back(cube);
		}
	}
	float spacing = 3.0f;
	for (int x = -1; x <= 2; ++x) {
		for (int z = -1; z <= 1; ++z) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(x * spacing, 1.5f, z * spacing));
			model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
			models.push_back(model);
			cubes.push_back(cube);
		}
	}
	glm::mat4 currentView, currentProj;
	if (useEasyCam) {
		ofPushView();
		ofViewport(viewportX, viewportY, renderW, renderH);
		easyCam.begin();
		currentView = easyCam.getModelViewMatrix();
		currentProj = easyCam.getProjectionMatrix();
		easyCam.end();
		ofPopView();
	}
	else {
		currentView = fixedView;
		currentProj = fixedProj;
	}
	
	deferred.shadowPass(cubes, models);
	deferred.geometryPass(cubes, models, currentView, currentProj);
	deferred.lightingPass(currentView, currentProj, viewportX, viewportY, renderW, renderH);
}

void DeferredRenderer::setFixedCameraPosition(const glm::vec3& position, const glm::vec3& target) {
	fixedView = glm::lookAt(position, target, glm::vec3(0, 1, 0));
}

void DeferredRenderer::resetFixedCamera() {
	setupFixedCamera();
}

glm::mat4 DeferredRenderer::getCurrentViewMatrix() const {
	return useEasyCam ? easyCam.getModelViewMatrix() : fixedView;
}

glm::mat4 DeferredRenderer::getCurrentProjectionMatrix() const {
	return useEasyCam ? easyCam.getProjectionMatrix() : fixedProj;
}
