#include "ImageManager.h"

void ImageManager::loadImage(const std::string& path) {
	ofImage img;
	if (img.load(path)) {
		images.clear();  // for now only keep the new image
		images.push_back(std::move(img));

		// Resize window to fit the latest image
		const auto& last = images.back();
		ofSetWindowShape(last.getWidth(), last.getHeight());
	}
}

const std::vector<ofImage>& ImageManager::getImages() const {
	return images;
}
