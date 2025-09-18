#include "Renderer.h"
void Renderer::setup(){}
void Renderer::setImageManager(ImageManager* manager) {
	imageManager = manager;
}
void Renderer::setShowHistogram(bool enabled) {
	showHistogram = enabled;

	if(showHistogram && imageManager && !imageManager->getImages().empty()) {
		histogram.calculateColours(imageManager->getImages().back());
	}
}
void Renderer::setGUIManager(GUIManager* guiManager) {
	gui = guiManager;
}
void Renderer::draw() {
	if(!imageManager) return;

	for(auto& img : imageManager->getImages()) {
		if(img.isAllocated())
			img.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	}

	if(showHistogram) {
		histogram.draw(10, 10, 256, 100);
	}

	if(gui) {
		gui->draw();
	}
}

