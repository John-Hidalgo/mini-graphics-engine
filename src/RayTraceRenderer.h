#pragma once
#include "ofMain.h"
#include <glm/glm.hpp>
#include <vector>

class RayTraceRenderer {
public:
	RayTraceRenderer(float leftPanel = 220.0f, float rightPanel = 540.0f, float bottomPanel = 160.0f);
	~RayTraceRenderer();
	
	void setup();
	void update();
	void draw();
	
	void enableEasyCam(bool enable) { useEasyCam = enable; }
	void toggleCamera() { useEasyCam = !useEasyCam; }
	ofEasyCam& getCamera() { return easyCam; }
	bool isUsingEasyCam() const { return useEasyCam; }
	void setFixedCameraPosition(const glm::vec3& position, const glm::vec3& target = glm::vec3(0, 0, 0));
	void resetFixedCamera();
	
	void addCube(const glm::vec3& center, const glm::vec3& size, const glm::vec3& color);
	void addSphere(const glm::vec3& center, float radius, const glm::vec3& color);
	void addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& color);
	
private:
	void loadShaders();
	void setupFullscreenQuad();
	void sendSceneDataToShader();
	void setupFixedCamera();
	void setupEasyCamera();
	GLuint shaderProgram;
	GLuint VAO, VBO, EBO;
	std::vector<glm::vec3> cubeCenters;
	std::vector<glm::vec3> cubeSizes;
	std::vector<glm::vec3> cubeColors;
	std::vector<glm::vec3> sphereCenters;
	std::vector<float> sphereRadii;
	std::vector<glm::vec3> sphereColors;
	std::vector<glm::vec3> triangleV0;
	std::vector<glm::vec3> triangleV1;
	std::vector<glm::vec3> triangleV2;
	std::vector<glm::vec3> triangleColors;
	ofEasyCam easyCam;
	glm::mat4 fixedView;
	glm::mat4 fixedProj;
	bool useEasyCam = true;
	float leftPanelWidth;
	float rightPanelWidth;
	float bottomPanelHeight;
	std::vector<GLint> cubeCenterLocs, cubeSizeLocs, cubeColorLocs;
	std::vector<GLint> sphereCenterLocs, sphereRadiusLocs, sphereColorLocs;
	std::vector<GLint> triangleV0Locs, triangleV1Locs, triangleV2Locs, triangleColorLocs;
	GLint resolutionLoc, timeLoc, cameraPosLoc, cameraTargetLoc;
	GLint numCubesLoc, numSpheresLoc, numTrianglesLoc;
};
