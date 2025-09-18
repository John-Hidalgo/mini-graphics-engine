#pragma once
#include "Renderer.h"
#include "ImageManager.h"
#include "GUIManager.h"
#include "Histogram.h"
#include <string>

class Application {
public:
	void setup();
	void update();
	void draw();

private:
	Renderer renderer;
	ImageManager imageManager;
	GUIManager gui;
	Histogram histogram;

	void handleImport(const std::string& path);
	void handleClear();
	void handleHistogram(bool enabled);
};
