#pragma once
#include "ofMain.h"  // Pour ofLight, ofColor, ofPoint
#include <glm/glm.hpp>

enum class LightType {
    LIGHT_AMBIENT, LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT
};

struct LightData {
    LightType type;
    std::unique_ptr<ofLight> light;;
    ofColor color;
    ofPoint position;
    glm::vec3 direction;
    float attenuation;
    bool enabled;

    explicit LightData(LightType t)
        : type(t),
          color(ofColor::white),
          position(0, 200, 0),
          direction(0, -1, 0),
          attenuation(1.0f),
          enabled(true) {}
};