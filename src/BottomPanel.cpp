#include "BottomPanel.h"

BottomPanel::BottomPanel()
        : x(0), y(ofGetHeight() - bottomPanelHeight), width(ofGetWidth()), height(bottomPanelHeight), panelPadding(10)
{}

void BottomPanel::setup(Canvas* canvas, const ofRectangle& area) {
    canvasRef = canvas;
    panelArea = area;

    x = area.x;
    y = area.y;
    width = area.width;
    height = area.height;

    gui.setup("Bottom Panel Du Scene");
    gui.setPosition(x + panelPadding, y + panelPadding);

    gui.setBackgroundColor(ofColor(50, 50, 50));
    gui.setBorderColor(ofColor(100, 100, 100));
    gui.setTextColor(ofColor(255, 255, 255));
    gui.setHeaderBackgroundColor(ofColor(50, 50, 50));
    gui.setDefaultWidth(width - 2 * panelPadding);
}

void BottomPanel::draw() {
    ofPushStyle();
    ofSetColor(50, 50, 50);
    ofFill();
    ofDrawRectangle(x, y, width, height);
    gui.draw();
    ofPopStyle();
}

void BottomPanel::mousePressed(int mx, int my, int button) {
    ofPushStyle();
    ofPopStyle();
}

void BottomPanel::setPanelArea(const ofRectangle& area) {
    panelArea = area;
    x = area.x;
    y = area.y;
    width = area.width;
    height = area.height;

    gui.setPosition(x + panelPadding, y + panelPadding);
    gui.setDefaultWidth(width - 2 * panelPadding);
}
