#pragma once
#include "ofMain.h"

class Skybox {
public:
	void setup();
	void draw(ofCamera& camera);
	
private:
	ofShader shader;
	ofVboMesh mesh;
	
	void createMesh();
	void loadShaders();
};
