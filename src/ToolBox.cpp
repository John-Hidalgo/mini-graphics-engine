#include "ToolBox.h"
#include "ofMain.h"

void ToolBox::setup(const std::string& title) {
	panel.setup(title);

	panel.add(importButton.setup("Import Image"));
	importButton.addListener(this, &ToolBox::handleImport);

	panel.add(clearButton.setup("Remove Image"));
	clearButton.addListener(this, &ToolBox::handleClear);

	panel.add(histrogramToggle.setup("Show Histogram", false));
	histrogramToggle.addListener(this, &ToolBox::handleHistrogram);
}

void ToolBox::draw() {
	panel.draw();
}

void ToolBox::handleImport() {
	ofFileDialogResult result = ofSystemLoadDialog("Choose an image");
	if (result.bSuccess && onImportClicked) {
		onImportClicked(result.getPath());
	}
}

void ToolBox::handleClear() {
	if(onClearClicked) {
		onClearClicked();
	}
}

void ToolBox::handleHistrogram(bool & enabled) { // called by ofxToggle
	if(onHistrogramToggled) {
		onHistrogramToggled(enabled); // pass toggle state to Application
	}
}
