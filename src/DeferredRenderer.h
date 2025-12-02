#pragma once
#include "ofMain.h"
#include "Mesh.h"
#include "Deferred.h"
#include <vector>
#include <glm/glm.hpp>

class DeferredRenderer {
public:
	DeferredRenderer(float leftPanel, float RightPanel, float BottomPanel);
	void setup();
	void update();
	void draw();
	void enableEasyCam(bool enable) { useEasyCam = enable; }
	void toggleCamera() { useEasyCam = !useEasyCam; }
	ofEasyCam& getCamera() { return easyCam; }
	bool isUsingEasyCam() const { return useEasyCam; }
	void setFixedCameraPosition(const glm::vec3& position, const glm::vec3& target = glm::vec3(0, 0, 0));
	void resetFixedCamera();

private:
	Mesh cube;
	Deferred deferred = Deferred(1700, 1024);
	ofEasyCam easyCam;
	
	float angle = 0.0f;
	glm::mat4 fixedView;
	glm::mat4 fixedProj;
	float leftPanelWidth = 220.0f;
	float rightPanelWidth = 540.0f;
	float bottomPanelHeight = 160.0f;
	bool useEasyCam = true;

	void setupFixedCamera();
	void setupEasyCamera();
	glm::mat4 getCurrentViewMatrix() const;
	glm::mat4 getCurrentProjectionMatrix() const;
};
