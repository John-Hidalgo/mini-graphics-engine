#include "DeferredRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

DeferredRenderer::DeferredRenderer(float leftPanel,float RightPanel, float BottomPanel)
	: leftPanelWidth(leftPanel), rightPanelWidth(RightPanel),bottomPanelHeight(BottomPanel){}

void DeferredRenderer::setup() {
	ofDisableArbTex();
	//ofEnableDepthTest();
	deferred.setup();
	cube = Mesh::Cube();

	proj = glm::perspective(glm::radians(45.0f), 1500.0f / 1000.0f, 0.1f, 100.0f);
	view = glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0,0,0), glm::vec3(0,1,0));
}

void DeferredRenderer::update() {
	angle += ofGetLastFrameTime();
}

void DeferredRenderer::draw() {
	ofDisableAlphaBlending();
	int windowW = ofGetWidth();
	int windowH = ofGetHeight();

	float scaleW = 0.529f;//(1700 - (rightPanelWidth + leftPanelWidth))/1700;
	float scaleH = 0.843f;//(1024 - bottomPanelWidth) 1024;

	int renderW = int(windowW * scaleW);
	int renderH = int(windowH * scaleH);

	int viewportX = leftPanelWidth;
	int viewportY = windowH - renderH;

	std::vector<Mesh> cubes(9, cube);
	std::vector<glm::mat4> models(9);
	int floorSize = 5;
	float cubeSize = 1.0f;
	for (int x = -floorSize; x <= floorSize; ++x) {
		for (int z = -floorSize; z <= floorSize; ++z) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f),
											 glm::vec3(x * cubeSize, -cubeSize / 2.0f, z * cubeSize));
			models.push_back(model);
			cubes.push_back(cube);
		}
	}
	float spacing = 3.0f;
	for (int x = -1; x <= 2; ++x) {
		for (int z = -1; z <= 1; ++z) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f),
											 glm::vec3(x * spacing, 1.5f, z * spacing));
			model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
			models.push_back(model);
			cubes.push_back(cube);
		}
	}
	deferred.shadowPass(cubes, models);
	deferred.geometryPass(cubes, models, view, proj);
	deferred.lightingPass(view, proj,viewportX, viewportY, renderW, renderH);
}
