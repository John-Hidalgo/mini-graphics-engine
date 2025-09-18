#include "ofMain.h"
#include "Application.h"

class ofApp : public ofBaseApp {
public:
	Application app;

	void setup() override { app.setup(); }
	void update() override { app.update(); }
	void draw() override { app.draw(); }
};

int main() {
	ofSetupOpenGL(800, 600, OF_WINDOW);
	ofRunApp(new ofApp());
}
