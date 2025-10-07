#pragma once
#include "ofMain.h"
#include "Canvas.h"
#include "Toolbar.h"
#include "SceneGraph.h"
#include "BottomPanel.h"
#include "LeftPanel.h"
#include "Model3D.h"
#include "CameraView.h"

enum Coordinates {
	EUCLIDEAN,
	SPHERICAL,
	HYPERBOLIC
};

class Application : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseMoved(int x, int y);

	void windowResized(int w, int h);
	
	void setupCameras();
	std::vector<CameraView> cameras;
	int activeCameraIndex = 0;
	int previewWidth = 200;
	int previewHeight = 150;
	void updateCameraViewports(int w, int h);
	void updateActiveCamera();
	void resetCamerasToSphere();
	void drawDebugAxes(float length, float thickness);
	void updateYawPitchFromCamera();
	void keyPressed(int key);
	bool orbitMode = false;
	bool sphericalCoordinates = false;
	bool sphericalTangentialMode = false;
	
	struct SphericalMovement {
			glm::vec3 position;
			glm::vec3 surfaceNormal;
			glm::vec3 lookDir;
			glm::vec3 upDir;
			glm::vec3 rightDir;
		};
		
	SphericalMovement getSphericalMovementData(ofCamera& cam);
	glm::vec3 calculateSphericalMovement(const SphericalMovement& data, float moveSpeed);
	void applyTangentialOrientation(ofCamera& cam, const SphericalMovement& data);
	void handleSphericalRadiusChange(ofCamera& cam, float moveSpeed);
	
	Coordinates coodinates = EUCLIDEAN;
	glm::vec3 hyperbolicTranslate(const glm::vec3& pos, const glm::vec3& dir, float scale);
	glm::vec3 toHyperbolicCoords(const glm::vec3& euclideanPos, float scale);
	glm::vec3 toEuclideanCoords(const glm::vec3& hyperbolicPos, float scale);
	void foo();
	
private:
	Canvas canvas;
	Toolbar toolbar;
	SceneGraph sceneGraph;
	BottomPanel bottomPanel;
	LeftPanel leftPanel;

	ofRectangle canvasArea;
	ofRectangle sceneGraphArea;
	ofRectangle leftPanelArea;
	ofRectangle bottomPanelArea;

	float leftPanelWidth = 224.0f;
	float rightPanelWidth = 500.0f;
	float bottomPanelHeight = 160.0f;

	float yaw = 0.0f;   // Orientation a gauche et a droite
	float pitch = 0.0f; // Orientation en bas et en haut
	float mouseSensitivity = 0.2f;
	bool mouseCaptured = false;

};
