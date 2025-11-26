#include "RayTraceRenderer.h"
#include "loadShader.h"
#include "ofMain.h"

RayTraceRenderer::RayTraceRenderer(float leftPanel, float rightPanel, float bottomPanel)
	: leftPanelWidth(leftPanel), rightPanelWidth(rightPanel), bottomPanelHeight(bottomPanel),
	  shaderProgram(0), VAO(0), VBO(0), EBO(0)
{
}

RayTraceRenderer::~RayTraceRenderer() {
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	if (shaderProgram != 0) glDeleteProgram(shaderProgram);
}

void RayTraceRenderer::setup() {
	VAO = 0;
	VBO = 0;
	EBO = 0;

	setupFullscreenQuad();
	loadShaders();

	addCube(glm::vec3(-3.0f, 1.0f, -6.0f), glm::vec3(1.5f), glm::vec3(1.0f, 0.2f, 0.2f));
	addCube(glm::vec3(0.0f, 0.5f, -8.0f), glm::vec3(1.0f), glm::vec3(1.0f, 1.0f, 0.2f));

	addSphere(glm::vec3(3.0f, 1.0f, -6.0f), 2.0f, glm::vec3(0.2f, 1.0f, 0.2f));
	addSphere(glm::vec3(1.0f, 0.8f, -4.0f), 0.7f, glm::vec3(1.0f, 0.5f, 0.0f));

	addTriangle(
		glm::vec3(-1.0f, 2.0f, -5.0f),
		glm::vec3(-2.5f, 0.0f, -5.0f),
		glm::vec3(0.5f, 0.0f, -5.0f),
		glm::vec3(0.2f, 0.2f, 1.0f)
	);

	addTriangle(
		glm::vec3(2.0f, 1.5f, -7.0f),
		glm::vec3(1.0f, 0.0f, -7.0f),
		glm::vec3(3.0f, 0.0f, -7.0f),
		glm::vec3(1.0f, 0.2f, 1.0f)
	);

	easyCam.setDistance(15.0f);
	easyCam.lookAt(glm::vec3(0.0f, 1.0f, 0.0f));
}

void RayTraceRenderer::loadShaders() {
	shaderProgram = loadShader("shaders/rayTracer_330_vs.glsl", "shaders/rayTracer_330_fs.glsl");
	if (shaderProgram == 0) {
		ofLogError("RayTraceRenderer") << "Failed to load shaders!";
		return;
	}
	resolutionLoc = glGetUniformLocation(shaderProgram, "iResolution");
	timeLoc = glGetUniformLocation(shaderProgram, "iTime");
	cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
	cameraTargetLoc = glGetUniformLocation(shaderProgram, "cameraTarget");
	numCubesLoc = glGetUniformLocation(shaderProgram, "numCubes");
	numSpheresLoc = glGetUniformLocation(shaderProgram, "numSpheres");
	numTrianglesLoc = glGetUniformLocation(shaderProgram, "numTriangles");
	cubeCenterLocs.clear();
	cubeSizeLocs.clear();
	cubeColorLocs.clear();
	sphereCenterLocs.clear();
	sphereRadiusLocs.clear();
	sphereColorLocs.clear();
	triangleV0Locs.clear();
	triangleV1Locs.clear();
	triangleV2Locs.clear();
	triangleColorLocs.clear();

	for (int i = 0; i < 10; i++) {
		cubeCenterLocs.push_back(glGetUniformLocation(shaderProgram, ("cubeCenters[" + std::to_string(i) + "]").c_str()));
		cubeSizeLocs.push_back(glGetUniformLocation(shaderProgram, ("cubeSizes[" + std::to_string(i) + "]").c_str()));
		cubeColorLocs.push_back(glGetUniformLocation(shaderProgram, ("cubeColors[" + std::to_string(i) + "]").c_str()));

		sphereCenterLocs.push_back(glGetUniformLocation(shaderProgram, ("sphereCenters[" + std::to_string(i) + "]").c_str()));
		sphereRadiusLocs.push_back(glGetUniformLocation(shaderProgram, ("sphereRadii[" + std::to_string(i) + "]").c_str()));
		sphereColorLocs.push_back(glGetUniformLocation(shaderProgram, ("sphereColors[" + std::to_string(i) + "]").c_str()));

		triangleV0Locs.push_back(glGetUniformLocation(shaderProgram, ("triangleV0[" + std::to_string(i) + "]").c_str()));
		triangleV1Locs.push_back(glGetUniformLocation(shaderProgram, ("triangleV1[" + std::to_string(i) + "]").c_str()));
		triangleV2Locs.push_back(glGetUniformLocation(shaderProgram, ("triangleV2[" + std::to_string(i) + "]").c_str()));
		triangleColorLocs.push_back(glGetUniformLocation(shaderProgram, ("triangleColors[" + std::to_string(i) + "]").c_str()));
	}

	ofLogNotice("RayTraceRenderer") << "Shaders loaded successfully";
}

void RayTraceRenderer::setupFullscreenQuad() {
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	float vertices[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f
	};

	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		ofLogError("RayTraceRenderer") << "OpenGL error in setupFullscreenQuad: " << error;
	}
	else {
		ofLogNotice("RayTraceRenderer") << "Fullscreen quad setup successfully, VAO: " << VAO;
	}
}

void RayTraceRenderer::setupFixedCamera() {
	fixedProj = glm::perspective(glm::radians(45.0f), 1500.0f / 1000.0f, 0.1f, 100.0f);
	fixedView = glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void RayTraceRenderer::setupEasyCamera() {
	easyCam.setDistance(15.0f);
	easyCam.setNearClip(0.1f);
	easyCam.setFarClip(100.0f);
	easyCam.setPosition(0, 10, 10);
	easyCam.lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void RayTraceRenderer::update() {
}

void RayTraceRenderer::draw() {
	if (VAO == 0 || shaderProgram == 0) {
		ofLogError("RayTraceRenderer") << "OpenGL resources not initialized!";
		return;
	}
	int windowW = ofGetWidth();
	int windowH = ofGetHeight();
	float scaleW = 0.56f;
	float scaleH = 0.839f;
	int renderW = int(windowW * scaleW);
	int renderH = int(windowH * scaleH);

	int viewportX = leftPanelWidth;
	int viewportY = windowH - renderH;
	glm::vec3 cameraPos, cameraTarget;
	if (useEasyCam) {
		ofPushView();
		ofViewport(viewportX, viewportY, renderW, renderH);
		easyCam.begin();
		cameraPos = easyCam.getPosition();
		cameraTarget = cameraPos + easyCam.getLookAtDir();
		easyCam.end();
		ofPopView();
	}
	else {
		glm::mat4 invView = glm::inverse(fixedView);
		cameraPos = glm::vec3(invView[3]);
		cameraTarget = glm::vec3(0, 0, 0);
	}
	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	glViewport(viewportX, viewportY, renderW, renderH);
	glUseProgram(shaderProgram);
	glUniform2f(resolutionLoc, (float)renderW, (float)renderH);
	glUniform1f(timeLoc, ofGetElapsedTimef());
	glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(cameraTargetLoc, cameraTarget.x, cameraTarget.y, cameraTarget.z);
	glUniform1i(numCubesLoc, (int)cubeCenters.size());
	glUniform1i(numSpheresLoc, (int)sphereCenters.size());
	glUniform1i(numTrianglesLoc, (int)triangleV0.size());
	for (int i = 0; i < cubeCenters.size(); i++) {
		if (cubeCenterLocs[i] != -1) {
			glUniform3fv(cubeCenterLocs[i], 1, &cubeCenters[i][0]);
			glUniform3fv(cubeSizeLocs[i], 1, &cubeSizes[i][0]);
			glUniform3fv(cubeColorLocs[i], 1, &cubeColors[i][0]);
		}
	}
	for (int i = 0; i < sphereCenters.size(); i++) {
		if (sphereCenterLocs[i] != -1) {
			glUniform3fv(sphereCenterLocs[i], 1, &sphereCenters[i][0]);
			glUniform1f(sphereRadiusLocs[i], sphereRadii[i]);
			glUniform3fv(sphereColorLocs[i], 1, &sphereColors[i][0]);
		}
	}
	for (int i = 0; i < triangleV0.size(); i++) {
		if (triangleV0Locs[i] != -1) {
			glUniform3fv(triangleV0Locs[i], 1, &triangleV0[i][0]);
			glUniform3fv(triangleV1Locs[i], 1, &triangleV1[i][0]);
			glUniform3fv(triangleV2Locs[i], 1, &triangleV2[i][0]);
			glUniform3fv(triangleColorLocs[i], 1, &triangleColors[i][0]);
		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(currentProgram);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		ofLogError("RayTraceRenderer") << "OpenGL error: " << error;
	}
}

void RayTraceRenderer::setFixedCameraPosition(const glm::vec3& position, const glm::vec3& target) {
	fixedView = glm::lookAt(position, target, glm::vec3(0, 1, 0));
}

void RayTraceRenderer::resetFixedCamera() {
	setupFixedCamera();
}

void RayTraceRenderer::addCube(const glm::vec3& center, const glm::vec3& size, const glm::vec3& color) {
	cubeCenters.push_back(center);
	cubeSizes.push_back(size);
	cubeColors.push_back(color);
}

void RayTraceRenderer::addSphere(const glm::vec3& center, float radius, const glm::vec3& color) {
	sphereCenters.push_back(center);
	sphereRadii.push_back(radius);
	sphereColors.push_back(color);
}

void RayTraceRenderer::addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& color) {
	triangleV0.push_back(v0);
	triangleV1.push_back(v1);
	triangleV2.push_back(v2);
	triangleColors.push_back(color);
}
