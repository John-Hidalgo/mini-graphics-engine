#pragma once
#include "ofMain.h"
#include "Canvas.h"
#include "ofxGui.h"

class LeftPanel {
public:
    LeftPanel();
    void setup(Canvas* canvas, const ofRectangle& area);
    void draw();
    void mousePressed(int x, int y, int button);
    void setPanelArea(const ofRectangle& area);

private:
    Canvas* canvasRef;
    ofRectangle panelArea;
    float sidebarWidth = 224.0f;
    float bottomPanelHeight = 100.0f;

    float x, y, width, height;
    float panelPadding = 10;

    ofxPanel gui;
    ofxFloatSlider thicknessSlider;
    ofxColorSlider colorSlider;
    ofxButton deleteButton;
    ofxFloatSlider hueSlider;
    ofxFloatSlider satSlider;
    ofxFloatSlider briSlider;
    ofxToggle contourToggle;
    ofxToggle fillToggle;
};
