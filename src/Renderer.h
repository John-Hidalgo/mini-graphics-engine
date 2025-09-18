#pragma once
#include "ofMain.h"
#include "Histogram.h"
#include "ImageManager.h"
#include "GUIManager.h"

class Renderer {
public:
	void setup();
	void setImageManager(ImageManager* manager);
	void setGUIManager(GUIManager* guiManager);
	void setShowHistogram(bool enabled);
	void draw();

private:
	ImageManager* imageManager = nullptr;
	GUIManager* gui = nullptr;
	Histogram histogram;
	bool showHistogram = false;
};

