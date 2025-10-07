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

//void Histogramme::draw(float x, float y, float width, float height) {
//	if (maxCount == 0) return;
//
//	float barWidth = width / 256.0f;
//
//	for (int i = 0; i < 256; i++) {
//		float rHeight = (histogramR[i] / (float)maxCount) * height;
//		float gHeight = (histogramG[i] / (float)maxCount) * height;
//		float bHeight = (histogramB[i] / (float)maxCount) * height;
//
//		ofSetColor(255, 0, 0, 150); // red
//		ofDrawRectangle(x + i * barWidth, y + height - rHeight, barWidth, rHeight);
//
//		ofSetColor(0, 255, 0, 150); // green
//		ofDrawRectangle(x + i * barWidth, y + height - gHeight, barWidth, gHeight);
//
//		ofSetColor(0, 0, 255, 150); // blue
//		ofDrawRectangle(x + i * barWidth, y + height - bHeight, barWidth, bHeight);
//	}
//
//	ofSetColor(255);
//}

void Histogramme::draw(float x, float y, float width, float height) {
	if (maxCount == 0) return;

	float sectionWidth = width / 3.0f;
	float barWidth = sectionWidth / 256.0f;

	// Red
	for (int i = 0; i < 256; i++) {
		float barHeight = (histogramR[i] / (float)maxCount) * height;
		ofSetColor(255, 0, 0);
		ofDrawRectangle(x + i * barWidth, y + height - barHeight, barWidth, barHeight);
	}

	// Green
	for (int i = 0; i < 256; i++) {
		float barHeight = (histogramG[i] / (float)maxCount) * height;
		ofSetColor(0, 255, 0);
		ofDrawRectangle(x + sectionWidth + i * barWidth, y + height - barHeight, barWidth, barHeight);
	}

	// Blue
	for (int i = 0; i < 256; i++) {
		float barHeight = (histogramB[i] / (float)maxCount) * height;
		ofSetColor(0, 0, 255);
		ofDrawRectangle(x + 2 * sectionWidth + i * barWidth, y + height - barHeight, barWidth, barHeight);
	}

	ofSetColor(255);
}

//void Histogramme::draw(float x, float y, float width, float height) {
//	if (maxCount == 0) return;
//
//	float barWidth = width / 256.0f / 3.0f;
//
//	for (int i = 0; i < 256; i++) {
//		float rHeight = (histogramR[i] / (float)maxCount) * height;
//		float gHeight = (histogramG[i] / (float)maxCount) * height;
//		float bHeight = (histogramB[i] / (float)maxCount) * height;
//
//		ofSetColor(255, 0, 0, 200); // red
//		ofDrawRectangle(x + i * barWidth * 3, y + height - rHeight, barWidth, rHeight);
//
//		ofSetColor(0, 255, 0, 200); // green
//		ofDrawRectangle(x + i * barWidth * 3 + barWidth, y + height - gHeight, barWidth, gHeight);
//
//		ofSetColor(0, 0, 255, 200); // blue
//		ofDrawRectangle(x + i * barWidth * 3 + barWidth * 2, y + height - bHeight, barWidth, bHeight);
//	}
//
//	ofSetColor(255);
//}

