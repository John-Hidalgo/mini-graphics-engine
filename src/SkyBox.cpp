#include "Skybox.h"

void Skybox::setup() {
	loadShaders();
	createMesh();
}

void Skybox::loadShaders() {
	shader.load("shaders/skybox_330_vs.glsl", "shaders/skybox_330_fs.glsl");
}

void Skybox::createMesh() {
	float size = 2000.0f;
	
	vector<ofVec3f> vertices = {
		ofVec3f(-size, -size, -size), ofVec3f(size, -size, -size), ofVec3f(size, size, -size),
		ofVec3f(size, size, -size), ofVec3f(-size, size, -size), ofVec3f(-size, -size, -size),
		
		ofVec3f(-size, -size, size), ofVec3f(-size, size, size), ofVec3f(size, size, size),
		ofVec3f(size, size, size), ofVec3f(size, -size, size), ofVec3f(-size, -size, size),
		
		ofVec3f(-size, -size, -size), ofVec3f(-size, size, -size), ofVec3f(-size, size, size),
		ofVec3f(-size, size, size), ofVec3f(-size, -size, size), ofVec3f(-size, -size, -size),
		
		ofVec3f(size, -size, -size), ofVec3f(size, -size, size), ofVec3f(size, size, size),
		ofVec3f(size, size, size), ofVec3f(size, size, -size), ofVec3f(size, -size, -size),
		
		ofVec3f(-size, size, -size), ofVec3f(size, size, -size), ofVec3f(size, size, size),
		ofVec3f(size, size, size), ofVec3f(-size, size, size), ofVec3f(-size, size, -size),
		
		ofVec3f(-size, -size, -size), ofVec3f(-size, -size, size), ofVec3f(size, -size, size),
		ofVec3f(size, -size, size), ofVec3f(size, -size, -size), ofVec3f(-size, -size, -size)
	};
	
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	
	for(auto& vertex : vertices) {
		mesh.addVertex(vertex);
	}
}

void Skybox::draw(ofCamera& camera) {
	glDepthMask(GL_FALSE);
	
	ofPushMatrix();
	ofTranslate(camera.getPosition());
	
	shader.begin();
	mesh.draw();
	shader.end();
	
	ofPopMatrix();
	glDepthMask(GL_TRUE);
}
