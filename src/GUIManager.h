#pragma once
#include "ToolBox.h"

class GUIManager {
public:
	void setup();
	void draw();

	ToolBox& getToolBox();

private:
	ToolBox toolbox;
};
