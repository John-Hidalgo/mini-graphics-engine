#pragma once
#include "ofxGui.h"
#include <functional>

class ToolBox {
public:
	void setup(const std::string& title = "Toolbox");
	void draw();

	std::function<void(const std::string&)> onImportClicked;
	std::function<void()> onClearClicked;
	std::function<void(bool)> onHistrogramToggled;

private:
	ofxPanel panel;
	ofxButton importButton;
	ofxButton clearButton;
	ofxToggle histrogramToggle;

	void handleImport();
	void handleClear();
	void handleHistrogram(bool& enabled);
};
