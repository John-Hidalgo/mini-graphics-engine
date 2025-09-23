#pragma once
#include "ofMain.h"
#include "Canvas.h"
#include "Toolbar.h"
#include "SceneGraph.h"

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

	ofRectangle canvasArea;
	ofRectangle sceneGraphArea;
};
