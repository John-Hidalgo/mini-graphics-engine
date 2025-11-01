#include "Deferred.h"
#include "loadShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Deferred::Deferred(int w, int h)
	: width(w), height(h), lightPos(5.0f, 3.0f, 5.0f) {}

void Deferred::setup() {
	setupGBuffer();
	setupShadowMap();
	loadShaders();
	setupQuad();
}

void Deferred::geometryPass(std::vector<Mesh> &meshes, std::vector<glm::mat4> &models,const glm::mat4 &view, const glm::mat4 &proj) {
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(gBufferShader);
	glUniformMatrix4fv(glGetUniformLocation(gBufferShader,"view"),1,GL_FALSE,glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(gBufferShader,"projection"),1,GL_FALSE,glm::value_ptr(proj));

	for(size_t i=0;i<meshes.size();++i){
		glUniformMatrix4fv(glGetUniformLocation(gBufferShader,"model"),1,GL_FALSE,glm::value_ptr(models[i]));
		meshes[i].Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Deferred::shadowPass(std::vector<Mesh> &meshes, std::vector<glm::mat4> &models) {
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShader);
	float halfX = 10.0f, halfY = 10.0f;
	float near_plane = 1.0f, far_plane = 30.0f;
	glm::mat4 lightProjection = glm::ortho(-halfX, halfX, -halfY, halfY, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0,1,0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	glUniformMatrix4fv(glGetUniformLocation(shadowShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	for(size_t i=0;i<meshes.size();++i){
		glUniformMatrix4fv(glGetUniformLocation(shadowShader, "model"), 1, GL_FALSE, glm::value_ptr(models[i]));
		meshes[i].Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

void Deferred::lightingPass(const glm::mat4 &view, const glm::mat4 &proj, int x, int y, int w, int h) {
	if (w == -1) w = width;
	if (h == -1) h = height;
	glViewport(x, y, w, h);
	//glEnable(GL_DEPTH_TEST);

	//glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(lightingShader);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gPosition);
	glUniform1i(glGetUniformLocation(lightingShader,"gPosition"), 0);
	glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gNormal);
	glUniform1i(glGetUniformLocation(lightingShader,"gNormal"), 1);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glUniform1i(glGetUniformLocation(lightingShader,"gAlbedo"), 2);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(glGetUniformLocation(lightingShader,"shadowMap"), 3);
	glUniform3fv(glGetUniformLocation(lightingShader,"lightPos"),1,glm::value_ptr(lightPos));
	float halfX = 10.0f, halfY = 10.0f;
	float near_plane = 1.0f, far_plane  = 30.0f;
	glm::mat4 lightProjection = glm::ortho(-halfX, halfX, -halfY, halfY, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0,1,0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	glUniformMatrix4fv(glGetUniformLocation(lightingShader,"lightSpaceMatrix"),1,GL_FALSE,glm::value_ptr(lightSpaceMatrix));
	renderQuad();
	glEnable(GL_DEPTH_TEST);
}

void Deferred::setupGBuffer() {
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,width,height,0,GL_RGB,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,gPosition,0);
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,width,height,0,GL_RGB,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,gNormal,0);
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,gAlbedo,0);
	GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);
	glGenRenderbuffers(1,&rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboDepth);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Deferred::setupShadowMap() {
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0,1.0,1.0,1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Deferred::loadShaders() {
	gBufferShader = loadShader("shaders/gBuffer_330_vs.glsl","shaders/gBuffer_330_fs.glsl");
	lightingShader = loadShader("shaders/lighting_330_vs.glsl","shaders/lighting_330_fs.glsl");
	shadowShader   = loadShader("shaders/shadow_330_vs.glsl","shaders/shadow_330_fs.glsl");
}

void Deferred::setupQuad() {
	if(quadVAO != 0) return;

	float quadVertices[] = {
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}

void Deferred::renderQuad() {
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
