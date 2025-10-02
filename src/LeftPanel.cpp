#include "LeftPanel.h"

LeftPanel::LeftPanel()
        : x(0), y(0), width(sidebarWidth), height(ofGetHeight() - bottomPanelHeight), panelPadding(10)
{}

void LeftPanel::setup(Canvas* canvas, const ofRectangle& area) {
    canvasRef = canvas;
    panelArea = area;

    x = area.x;
    y = area.y;
    width = area.width;
    height = area.height;

    gui.setup("Left Panel Du Scene");
    gui.setPosition(x + panelPadding, y + panelPadding);

    gui.setBackgroundColor(ofColor(50, 50, 50));
    gui.setBorderColor(ofColor(100, 100, 100));
    gui.setTextColor(ofColor(255, 255, 255));
    gui.setHeaderBackgroundColor(ofColor(50, 50, 50));
	gui.setSize(200, 0);
}

void LeftPanel::draw() {
    ofPushStyle();
    ofSetColor(50, 50, 50);
    ofFill();
    ofDrawRectangle(x, y, width, height);
    gui.draw();
    ofPopStyle();
}

void LeftPanel::mousePressed(int mx, int my, int button) {
    ofPushStyle();
    ofPopStyle();
}


void LeftPanel::setPanelArea(const ofRectangle& area) {
    panelArea = area;
    x = area.x;
    y = area.y;
    width = area.width;
    height = area.height;

    gui.setPosition(x + panelPadding, y + panelPadding);
    gui.setDefaultWidth(width - 2 * panelPadding);
}
