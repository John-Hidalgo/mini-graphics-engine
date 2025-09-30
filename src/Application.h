#pragma once
#include "ofMain.h"
#include "Canvas.h"
#include "Toolbar.h"
#include "SceneGraph.h"
#include "BottomPanel.h"
#include "LeftPanel.h"
#include "Model3D.h"

class Application : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);

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
	float bottomPanelHeight = 100.0f;
};
