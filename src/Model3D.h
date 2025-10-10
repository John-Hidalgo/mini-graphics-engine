#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

enum class ModelVariant {
	None = 0,
	Metallic,
	Plastic,
	Wireframe,
	Transparent
};

class Model3D {
public:
	ofShader shader;
	ofShader shader_lambert;
	ofShader shader_normal;

	ofLight light;

	ofxAssimpModelLoader model;

	ofColor color_background;
	ofColor color_ambient;
	ofColor color_diffuse;
	
	ofVec3f position;

	ModelVariant variant = ModelVariant::None;
	ofTexture texture;

	float center_x;
	float center_y;

	float scale_model;

	float rotation_speed;

	bool use_rotation;

	void setup();
	void applyVariant(ModelVariant variant);
	void update();
	void draw();
	void loadModel(const std::string & path);
};
