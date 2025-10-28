#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "BoundingBox.h"

enum class ModelVariant {
	None = 0,
	Metallic,
	Plastic,
	Wireframe,
	Transparent
};
enum class Lighting{
	LAMBERT,
	GOURAUD,
	PHONG,
	BLINNPHONG,
	CELL
};

enum class ProceduralTexture {
		NONE,
		INVERSION,
		WEIERSTRASS
	};


class Model3D {
public:
	ofShader shader;
	ofShader shader_lambert;
	ofShader shader_normal;
	ofShader shader_gouraud;
	ofShader shader_phong;
	ofShader shader_blinnPhong;
	ofShader shader_cell;
	ofShader shader_inversion;
	ofShader shader_weierstrass;
	
	ofLight light;

	BoundingBox bbox;
	bool showBoundingBox = false;

	ofxAssimpModelLoader model;

	ofColor color_background;
	ofColor color_ambient;
	ofColor color_diffuse;
	ofParameter<int> celBands;
	ofParameter<float> celSpecularSize;
	ofParameter<float> celOutlineWidth;
	ofParameter<ofColor> celOutlineColor;
	
	ofVec3f position;

	ModelVariant variant = ModelVariant::None;
	Lighting ligthing = Lighting::LAMBERT;
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
	void setShader(Lighting lighting);
	void loadModel(const std::string & path);
	void drawBoundingBox();
	
	void setProceduralTexture(ProceduralTexture texture);
	void toggleProceduralTexture(ProceduralTexture texture);

	Lighting getCurrentLighting() const { return currentLighting; }
	ProceduralTexture getCurrentTexture() const { return currentTexture; }

	Lighting currentLighting;
	ProceduralTexture currentTexture;
	Lighting previousLighting;
};
