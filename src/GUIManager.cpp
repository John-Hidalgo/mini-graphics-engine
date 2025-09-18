#include "GUIManager.h"

void GUIManager::setup() {
	toolbox.setup();
}

void GUIManager::draw() {
	toolbox.draw();
}

ToolBox& GUIManager::getToolBox() {
	return toolbox;
}
