#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "BoundingBox.h"
#include "Light.h"

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

enum class Texture {
	NONE,
	INVERSION,
	WEIERSTRASS,
	NORMAL_MAPPING,
	HDR_DAY,
	HDR_NIGHT
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
	ofShader shader_textured;
	ofShader shader_displacement;
	ofTexture colorTexture;
	ofTexture normalMap;
	glm::vec3 lightPos;
	ofTexture displacementMap;
	ofEasyCam cam;
	ofLight light;

	BoundingBox bbox;
	bool showBoundingBox = false;
	bool enableNormalMapping = true;
	ofxAssimpModelLoader model;

	ofColor color_background;
	ofColor color_ambient;
	ofColor color_diffuse;
	ofParameter<int> celBands;
	ofParameter<float> celSpecularSize;
	ofParameter<float> celOutlineWidth;
	ofParameter<ofColor> celOutlineColor;
	bool animateSurface = false;
	bool animateColour = false;
	ofParameter<float> animationSpeed{"Animation Speed", 1.0f, 0.1f, 5.0f};
	ofParameter<float> rippleAmplitude{"Ripple Amplitude", 0.3f, 0.0f, 1.0f};
	ofParameter<float> colorSpeed{"Colour Speed", 1.0f, 0.1f, 3.0f};
	ofVec3f position;

	ModelVariant variant = ModelVariant::None;
	Lighting ligthing = Lighting::LAMBERT;
	ofTexture texture;

	float center_x;
	float center_y;
	float scale_model;
	float rotation_speed = 0.2f;
	bool use_rotation = true;

	void setup();
	void setupTextures();
	void applyVariant(ModelVariant variant);
	void update();
	void draw(const std::vector<LightData>& lights);
	void setShader(Lighting lighting);
	void loadModel(const std::string & path);
	void drawBoundingBox();
	void generateSphericalUVs(ofMesh& mesh);
	
	void setProceduralTexture(Texture texture);
	void toggleProceduralTexture(Texture texture);

	Lighting getCurrentLighting() const { return currentLighting; }
	Texture getCurrentTexture() const { return currentTexture; }
	bool getEnableNormalMapping() {return enableNormalMapping;}
	void setAnimateSurface(bool val) {animateSurface = val;}
	void setAnimateColour(bool val) {animateColour = val;}

	Lighting currentLighting;
	Texture currentTexture;
	Lighting previousLighting;
	float displacementScale = 50.0f;
	std::map<std::string, float> modelDisplacementDefaults;

	ofShader shaderHDRDay;
	ofShader shaderHDRNight;
	ofCubeMap cubeMapDay;
	ofCubeMap cubeMapNight;

	// 7.2 Pour les materials
	ofMaterial material;
	bool isMaterialActive = false;
};
