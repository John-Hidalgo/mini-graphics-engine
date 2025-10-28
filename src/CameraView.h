#include "ofEasyCam.h"
#include "BottomPanel.h"
#include "Canvas.h"

class CameraView {
public:
	CameraView();

	//ofEasyCam cam;
	ofCamera cam;
	ofRectangle viewport;
	void setup();
	void drawCameraOnCanvas();
	void drawCamerasOnBottomPanel();
};
