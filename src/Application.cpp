#include "Application.h"

void Application::setup() {
	renderer.setup();
	renderer.setImageManager(&imageManager);
	renderer.setGUIManager(&gui);
	gui.setup();

	gui.getToolBox().onImportClicked = [this](const std::string& path) {
		handleImport(path);
	};
	gui.getToolBox().onClearClicked = [this]() {
		handleClear();
	};
	gui.getToolBox().onHistrogramToggled = [this](bool enabled) {
		handleHistogram(enabled);
	};
}

void Application::update() {
}

void Application::draw() {
	renderer.draw();

	//gui.draw();
}

void Application::handleImport(const std::string& path) {
	imageManager.loadImage(path);

	if(!imageManager.getImages().empty()) {
		histogram.calculateColours(imageManager.getImages().back());
	}
}

void Application::handleClear() {
	auto& images = const_cast<std::vector<ofImage>&>(imageManager.getImages());
	images.clear();
	ofSetWindowShape(800, 600);
}

void Application::handleHistogram(bool enabled) {
	renderer.setShowHistogram(enabled);
}
