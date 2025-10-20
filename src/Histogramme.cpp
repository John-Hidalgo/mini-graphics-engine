#include "Histogramme.h"

void Histogramme::calculateColours(const ofImage &img) {
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

void Histogramme::draw(float x, float y, float width, float height) {
	if (maxCount == 0) return;

	float barWidth = width / 256.0f;

	for (int i = 0; i < 256; i++) {
		float rHeight = (histogramR[i] / (float)maxCount) * height;
		float gHeight = (histogramG[i] / (float)maxCount) * height;
		float bHeight = (histogramB[i] / (float)maxCount) * height;

		ofSetColor(255, 0, 0);
		ofDrawRectangle(x + i * barWidth, y + height - rHeight, barWidth, rHeight);

		ofSetColor(0, 255, 0);
		ofDrawRectangle(x + i * barWidth, y + height - rHeight - gHeight, barWidth, gHeight);

		ofSetColor(0, 0, 255);
		ofDrawRectangle(x + i * barWidth, y + height - rHeight - gHeight - bHeight, barWidth, bHeight);
	}

	ofSetColor(255);
}

