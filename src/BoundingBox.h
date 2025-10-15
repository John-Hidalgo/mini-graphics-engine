#pragma once
#include "ofMain.h"

struct BoundingBox {
    ofVec3f min;
    ofVec3f max;

    BoundingBox() {
        reset();
    }

    void reset() {
        min.set(FLT_MAX);
        max.set(-FLT_MAX);
    }

    void expandToInclude(const ofVec3f& point) {
        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);
        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }

    ofVec3f getCenter() const {
        return (min + max) * 0.5f;
    }

    ofVec3f getSize() const {
        return max - min;
    }

    void draw() const {
        ofNoFill();
        ofSetColor(0, 255, 0);
        ofDrawBox(getCenter(), getSize().x, getSize().y, getSize().z);
    }
};