#pragma once
#include "ofMain.h"

class Histogramme {
public:
	void calculateColours(const ofImage &img);
	void draw(float x, float y, float width, float height);

private:
	int histogramR[256] = {0};
	int histogramG[256] = {0};
	int histogramB[256] = {0};
	int maxCount = 0;
};
