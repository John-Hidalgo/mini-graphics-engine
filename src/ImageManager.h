#pragma once
#include "ofMain.h"
#include <vector>

class ImageManager {
public:
	void loadImage(const std::string& path);
	const std::vector<ofImage>& getImages() const;

private:
	std::vector<ofImage> images;
};
