#pragma once
#include "ofMain.h"
#include "Mesh.h"
#include "Deferred.h"
#include <vector>
#include <glm/glm.hpp>

class DeferredRenderer {
public:
	DeferredRenderer(float leftPanel,float RightPanel, float BottomPanel);
	void setup();
	void update();
	void draw();

private:
	Mesh cube;
	Deferred deferred = Deferred(1000, 700);
	
	float angle = 0.0f;
	glm::mat4 view;
	glm::mat4 proj;
	float leftPanelWidth = 220.0f;
	float rightPanelWidth = 540.0f;
	float bottomPanelHeight = 160.0f;

};
