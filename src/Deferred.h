#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include <vector>

class Deferred {
public:
	Deferred(int w, int h);
	void setup();
	void shadowPass(std::vector<Mesh> &meshes, std::vector<glm::mat4> &models);
	void geometryPass(std::vector<Mesh> &meshes, std::vector<glm::mat4> &models,const glm::mat4 &view, const glm::mat4 &proj);
	void lightingPass(const glm::mat4 &view, const glm::mat4 &proj,int x = 0, int y = 0, int w = -1, int h = -1);
	glm::vec3 lightPos;
private:
	GLuint gBuffer, gPosition, gNormal, gAlbedo, rboDepth;
	GLuint gBufferShader, lightingShader, shadowShader;
	GLuint depthMapFBO = 0;
	GLuint depthMap = 0;
	const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	GLuint quadVAO = 0, quadVBO;
	int width, height;
	

	void setupGBuffer();
	void setupShadowMap();
	void setupQuad();
	void loadShaders();
	void renderQuad();
	
};
