#pragma once
#include "ofMain.h"
#include "Canvas.h"
#include "Toolbar.h"
#include "SceneGraph.h"
#include "BottomPanel.h"
#include "LeftPanel.h"
#include "Model3D.h"
#include "CameraView.h"

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
