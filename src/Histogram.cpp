#include "Histogram.h"

void Histogram::calculateColours(const ofImage &img) {
	std::fill(std::begin(histogramR), std::end(histogramR), 0);
	std::fill(std::begin(histogramG), std::end(histogramG), 0);
	std::fill(std::begin(histogramB), std::end(histogramB), 0);
	maxCount = 0;
	if (!img.isAllocated()) return;

	for (int y = 0; y < img.getHeight(); y++) {
		for (int x = 0; x < img.getWidth(); x++) {
			ofColor c = img.getColor(x, y);
			histogramR[c.r]++;
			histogramG[c.g]++;
			histogramB[c.b]++;
		}
	}

	for (int i = 0; i < 256; i++) {
		maxCount = std::max(maxCount, histogramR[i]);
		maxCount = std::max(maxCount, histogramG[i]);
		maxCount = std::max(maxCount, histogramB[i]);
	}
}

void Histogram::draw(float x, float y, float width, float height) {
	if (maxCount == 0) return;

	float barWidth = width / 256.0f;

	for (int i = 0; i < 256; i++) {

		float rHeight = log(1 + histogramR[i]) / log(1 + maxCount) * height;
		float gHeight = log(1 + histogramG[i]) / log(1 + maxCount) * height;
		float bHeight = log(1 + histogramB[i]) / log(1 + maxCount) * height;

		float bottom = y + height;

		ofSetColor(255, 0, 0, 200); // red
		ofDrawRectangle(x + i * barWidth, bottom - rHeight, barWidth, rHeight);
		bottom -= rHeight;

		ofSetColor(0, 255, 0, 200); // green
		ofDrawRectangle(x + i * barWidth, bottom - gHeight, barWidth, gHeight);
		bottom -= gHeight;

		ofSetColor(0, 0, 255, 200); // blue
		ofDrawRectangle(x + i * barWidth, bottom - bHeight, barWidth, bHeight);
	}

	ofSetColor(255);
}


