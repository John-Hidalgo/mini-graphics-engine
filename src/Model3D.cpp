#include "Model3D.h"

void Model3D::setup()
{
	ofSetFrameRate(60);
	//ofSetWindowShape(512, 512);
	//ofSetLogLevel(OF_LOG_VERBOSE);

	// paramètres
	scale_model = 0.2f;
	rotation_speed = 0.3f;
	use_rotation = false;
	// chargement du shader
	shader_lambert.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
	shader_normal.load("draw_normal_330_vs.glsl", "draw_normal_330_fs.glsl");
	// sélectionner le shader courant
	shader = shader_lambert;
  
}



void Model3D::update()
{
	// position au centre de la fenêtre d'affichage
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;

	// transformation du model
	model.setScale(scale_model, scale_model, scale_model);
	model.setPosition(position.x, position.y, position.z);
	//model.setPosition(0, 0, 0);
	if (use_rotation)
	model.setRotation(0, ofGetFrameNum() * rotation_speed, 0.0f, 1.0f, 0.0f);

	// configuration de la lumière
	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(center_x, center_y, 255.0f);
}

void Model3D::draw()
{
	// couleur de l'arrière-plan
	ofSetBackgroundColor(color_background.r, color_background.g, color_background.b);

	// activer l'occlusion en profondeur
	ofEnableDepthTest();

	// activer l'éclairage dynamique
	ofEnableLighting();

	// activer la lumière
	light.enable();

	// activer le shader
	shader.begin();

	// passer les attributs uniformes du shader
	shader.setUniform3f("color_ambient",  color_ambient.r / 255.0f, color_ambient.g / 255.0f, color_ambient.b / 255.0f);
	shader.setUniform3f("color_diffuse",  color_diffuse.r / 255.0f, color_diffuse.g / 255.0f, color_diffuse.b / 255.0f);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	if (variant == ModelVariant::Transparent) {
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	}

	// dessiner le model
	model.draw(OF_MESH_FILL);

	if (variant == ModelVariant::Transparent) {
		ofDisableBlendMode();
	}

	// désactiver le shader
	shader.end();

	// désactiver la lumière
	light.disable();

	// désactiver l'éclairage dynamique
	ofDisableLighting();

	// désactiver l'occlusion en profondeur
	ofDisableDepthTest();
}

void Model3D::applyVariant(ModelVariant chosenVariant) {
	variant = chosenVariant;
	switch (chosenVariant) {
	case ModelVariant::Metallic:
		ofLogNotice() << "Metallic model";
		color_ambient = ofColor(80, 80, 100);
		color_diffuse = ofColor(200, 200, 255);
		shader = shader_lambert;
		break;

	case ModelVariant::Plastic:
		ofLogNotice() << "Plastic model";
		color_ambient = ofColor(40, 0, 0);
		color_diffuse = ofColor(240, 60, 60);
		shader = shader_lambert;
		break;

	case ModelVariant::Wireframe:
		ofLogNotice() << "Wireframe model";
		color_ambient = ofColor(0, 0, 0);
		color_diffuse = ofColor(255, 255, 255);
		shader = shader_normal;
		break;

	case ModelVariant::Transparent:
		ofLogNotice() << "Transparent model";
		color_ambient = ofColor(100, 100, 100);
		color_diffuse = ofColor(255, 255, 255, 120);
		shader = shader_lambert;
		break;

	default:
		shader = shader_lambert;
		color_ambient = ofColor(50, 50, 50);
		color_diffuse = ofColor(200, 200, 200);
		break;
	}

	ofLogNotice() << "applyVariant() Ambient: "
			  << color_ambient << " Diffuse: " << color_diffuse;
}


void Model3D::loadModel(const std::string& path){
	std::string modelPath = path.empty() ? "enneperSurface.obj" : path;
	model.clear();
	if(model.load(ofToDataPath(modelPath, true))) {
		ofLogNotice() << "Loaded model: " << ofToDataPath(modelPath, true);
		model.disableMaterials();

		if(model.hasMeshes()) {
			ofLogNotice() << "Model has " << model.getNumMeshes() << " meshes";
		} else {
			ofLogError() << "Model has no meshes after loading!";
		}
	} else {
		ofLogError() << "Failed to load model: " << modelPath;
	}
}

