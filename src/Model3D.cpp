#include "Model3D.h"

#include "Light.h"

void Model3D::setup()
{
	ofSetFrameRate(60);
	scale_model = 0.2f;
	rotation_speed = 0.3f;
	use_rotation = false;
	shader_lambert.load("shaders/lambert_330_vs.glsl", "shaders/lambert_330_fs.glsl");
	shader_gouraud.load("shaders/gouraud_330_vs.glsl", "shaders/gouraud_330_fs.glsl");
	shader_phong.load("shaders/phong_330_vs.glsl", "shaders/phong_330_fs.glsl");
	shader_blinnPhong.load("shaders/blinn_phong_330_vs.glsl", "shaders/blinn_phong_330_fs.glsl");
	shader_cell.load("shaders/CellShaded_330_vertex.glsl", "shaders/CellShaded_330_fragment.glsl");
	shader_inversion.load("shaders/CircleInversion_330_vertex.glsl","shaders/CircleInversion_330_fragment.glsl");
	shader_weierstrass.load("shaders/Weierstrass_330_vertex.glsl","shaders/Weierstrass_330_fragment.glsl");
	shader = shader_lambert;
	currentLighting = Lighting::LAMBERT;
	currentTexture = Texture::NONE;
	previousLighting = Lighting::LAMBERT;
	celBands.set("Color Bands", 3, 2, 6);
	celSpecularSize.set("Specular Size", 0.3, 0.5, 1.0);
	celOutlineWidth.set("Outline Threshold", 0.1, 0.0, 1.0);
	celOutlineColor.set("Outline Color", ofColor(0, 0, 0));
	animationSpeed = 1.0f;
	rippleAmplitude = 0.1f;
	colorSpeed = 1.0f;
	modelDisplacementDefaults["moebiusStrip2.obj"] = 0.4f;
	modelDisplacementDefaults["teapot3.obj"]  = 0.3f;
	modelDisplacementDefaults["sphere2.obj"]  = 50.0f;
	modelDisplacementDefaults["cube.obj"]  = 50.0f;
	setupTextures();
}
void Model3D::setupTextures() {
	ofDisableArbTex();
	if (!ofLoadImage(colorTexture, "textures/StoneBricksSplitface001_COL_2K.jpg"))
		ofLogError() << "Failed to load color texture";
	if (!ofLoadImage(normalMap, "textures/StoneBricksSplitface001_NRM_2K.png"))
		ofLogError() << "Failed to load normal map";
	if (!ofLoadImage(displacementMap, "textures/StoneBricksSplitface001_DISP_2K.png"))
		ofLogError() << "Failed to load displacement map";
	cubeMapDay.load("images/qwantani_sunset_puresky_4k.hdr", 512,true);
	cubeMapNight.load("images/satara_night_4k.hdr", 512,true);

	shaderHDRDay.load("shaders/hdri_330_vs.glsl", "shaders/hdri_330_fs.glsl");
	shaderHDRNight.load("shaders/hdri_330_vs.glsl", "shaders/hdri_330_fs.glsl");
	color_ambient = ofColor(50, 50, 50);
	color_diffuse = ofColor(200, 200, 200);
	shader_normal.load("shaders/normalMapping_330_vs.glsl", "shaders/normalMapping_330_fs.glsl");
}
void Model3D::update()
{
	// position au centre de la fenêtre d'affichage
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;

	// transformation du model
	model.setScale(scale_model, scale_model, scale_model);
	model.setPosition(position.x, position.y, position.z);
	
	if (use_rotation)
		model.setRotation(0, ofGetFrameNum() * rotation_speed, 0.0f, 1.0f, 0.0f);

	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(center_x, center_y - 300.0f, 500.0f);
}
void Model3D::setShader(Lighting lighting)
{
	currentLighting = lighting;
	currentTexture = Texture::NONE;
	
	switch (lighting) {
		case Lighting::LAMBERT:
			shader = shader_lambert;
			break;
		case Lighting::GOURAUD:
			shader = shader_gouraud;
			break;
		case Lighting::PHONG:
			shader = shader_phong;
			break;
		case Lighting::BLINNPHONG:
			shader = shader_blinnPhong;
			break;
		case Lighting::CELL:
			shader = shader_cell;
			break;
		default:
			shader = shader_lambert;
			break;
	}
	//ofLogNotice() << "Set shader to: " << static_cast<int>(lighting);
}

void Model3D::setProceduralTexture(Texture texture)
{
	if (texture == currentTexture && texture != Texture::NONE) {
		toggleProceduralTexture(texture);
		return;
	}
	if (texture != Texture::NONE && currentTexture == Texture::NONE) {
		previousLighting = currentLighting;
	}
	
	currentTexture = texture;
	
	switch (texture) {
		case Texture::INVERSION:
			shader = shader_inversion;
			//ofLogNotice() << "Applied Circle Inversion texture over " << static_cast<int>(previousLighting);
			break;
		case Texture::WEIERSTRASS:
			shader = shader_weierstrass;
			//ofLogNotice() << "Applied Weierstrass texture over " << static_cast<int>(previousLighting);
			break;
		case Texture::NORMAL_MAPPING:
			if (!enableNormalMapping) {
				//ofLogWarning() << "Normal mapping not supported for this model — ignoring.";
				currentTexture = Texture::NONE;
				return;
			}
			//ofLogNotice() << "Applied normal mapping texture over " << static_cast<int>(previousLighting);
			shader = shader_normal;
			break;
		case Texture::HDR_DAY:
			shader = shaderHDRDay;
			break;
		case Texture::HDR_NIGHT:
			shader = shaderHDRNight;
			break;
		case Texture::NONE:
			setShader(previousLighting);
			//ofLogNotice() << "Removed procedural texture, restored to " << static_cast<int>(previousLighting);
			break;
	}
}

void Model3D::toggleProceduralTexture(Texture texture)
{
	currentTexture == texture?
		setProceduralTexture(Texture::NONE):
		setProceduralTexture(texture);
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
	//ofLogNotice() << "applyVariant() Ambient: " << color_ambient << " Diffuse: " << color_diffuse;
}
void Model3D::loadModel(const std::string& path) {
	std::string modelPath = path.empty() ? "enneperSurface.obj" : path;
	model.clear();

	if (model.load(ofToDataPath(modelPath, true))) {
		model.disableMaterials();
		std::string filename = ofFilePath::getFileName(modelPath);
		if(modelDisplacementDefaults.count(filename)) {
			displacementScale = modelDisplacementDefaults[filename];
			//ofLogNotice() << "Set displacement scale for " << filename << " to " << displacementScale;
			enableNormalMapping = true;
		}
		else {
			enableNormalMapping = false;
			currentTexture = Texture::NONE;
		}

		if (model.hasMeshes()) {
			const ofMesh& firstMesh = model.getMesh(0);
			if (firstMesh.hasTexCoords()) {
				//ofLogNotice() << "Model has texture coordinates - normal mapping will work!";
			} else {
				//ofLogWarning() << "Model has no texture coordinates - normal mapping won't work";
			}
		}
	}
}

void Model3D::drawBoundingBox(){
	if (showBoundingBox) {
		ofPushMatrix();
		ofMultMatrix(model.getModelMatrix());
		ofPushStyle();
		ofNoFill();
		ofSetColor(0, 255, 0);
		ofSetLineWidth(2);
		bbox.draw();
		ofPopStyle();
		ofPopMatrix();
	}
}
void Model3D::draw(const std::vector<LightData>& lights = {})
{
	ofSetBackgroundColor(color_background.r, color_background.g, color_background.b);
	ofEnableDepthTest();
	
	if (currentTexture == Texture::NONE) {
		ofEnableLighting();
		light.enable();
	}
	ofMatrix4x4 modelMatrix = model.getModelMatrix();
	ofMatrix4x4 viewMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
	ofMatrix4x4 projectionMatrix = ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
	ofMatrix4x4 mvp = projectionMatrix * viewMatrix * modelMatrix;
	shader.begin();
	
	shader.setUniformMatrix4f("modelViewMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
	shader.setUniformMatrix4f("projectionMatrix", ofGetCurrentMatrix(OF_MATRIX_PROJECTION));
	if (currentTexture == Texture::NONE) {
		shader.setUniform1f("u_time", ofGetElapsedTimef());
		shader.setUniform1f("u_animationSpeed", animationSpeed);
		shader.setUniform1f("u_rippleAmplitude", rippleAmplitude);
		shader.setUniform1f("u_colorSpeed", colorSpeed);
		shader.setUniform1i("u_animateSurface", animateSurface);
		shader.setUniform1i("u_animateColour", animateColour);
		shader.setUniform3f("color_ambient", color_ambient.r / 255.0f, color_ambient.g / 255.0f, color_ambient.b / 255.0f);
		shader.setUniform3f("color_diffuse", color_diffuse.r / 255.0f, color_diffuse.g / 255.0f, color_diffuse.b / 255.0f);
		//shader.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader.setUniform1f("brightness", 0.5f);
		if (!lights.empty()) {
			glm::vec3 combinedPos(0.0f);
			glm::vec3 combinedColor(0.0f);
			float totalAttenuation = 0.0f;

			for (const auto& l : lights) {
				float weight = l.attenuation;
				combinedPos += glm::vec3(l.position.x, l.position.y, l.position.z) * weight;
				combinedColor += glm::vec3(l.color.r / 255.0f, l.color.g / 255.0f, l.color.b / 255.0f) * weight;
				totalAttenuation += weight;
			}

			combinedPos /= totalAttenuation;
			combinedColor /= totalAttenuation;

			shader_lambert.setUniform3f("light_position", combinedPos);
			shader_lambert.setUniform3f("color_diffuse", combinedColor);
		} else {
			// Pas de lumière dynamique, utiliser canvasLight
			shader_lambert.setUniform3f("light_position", light.getGlobalPosition());
		}
		if(shader == shader_cell) {
			shader.setUniform1i("bands", celBands);
			shader.setUniform1f("outline_threshold", celOutlineWidth);
			shader.setUniform3f("outline_color",celOutlineColor->r / 255.0f, celOutlineColor->g / 255.0f, celOutlineColor->b / 255.0f);
			shader.setUniform3f("color_specular", 1.0f, 1.0f, 1.0f);
		}
		else if(shader != shader_lambert){
			shader.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		}
	}
	else if(currentTexture == Texture::HDR_DAY || currentTexture == Texture::HDR_NIGHT){
		//ofLog() << "here";
		shader.setUniformMatrix4f("modelViewProjectionMatrix", mvp);
		shader.setUniformMatrix4f("modelMatrix", modelMatrix);
		shader.setUniform1f("brightness", 1.5f); // 1.5x brighter
		shader.setUniform1f("specularStrength", 0.3f); // Specular amount
		shader.setUniform1f("exposure", 2.0f); // Higher exposure
		if(currentTexture == Texture::HDR_DAY){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapDay.getTextureId());
			shader.setUniform1i("environmentMap", 0);
		}
		else if(currentTexture == Texture::HDR_NIGHT){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNight.getTextureId());
			shader.setUniform1i("environmentMap", 0);
		}
	}
	else {
		shader.setUniform1f("u_time", ofGetElapsedTimef());
		shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());

		if(enableNormalMapping){
			shader.setUniformTexture("colorTexture", colorTexture, 0);
			shader.setUniformTexture("normalMap", normalMap, 1);
			shader.setUniformTexture("displacementMap", displacementMap, 2);
			shader.setUniform3f("lightPosition", glm::vec3(0.0, 200.0, 400.0));
			shader.setUniform1f("displacementScale", displacementScale);
		}
	}
	model.draw(OF_MESH_FILL);
	drawBoundingBox();
	shader.end();

	if (currentTexture == Texture::NONE) {
		light.disable();
		ofDisableLighting();
	}
	ofDisableDepthTest();
}

//void Model3D::draw() {
//	ofEnableDepthTest();
//	//shader = shaderHDRNight;
//
//	ofPushMatrix();
//	ofMatrix4x4 modelMatrix = model.getModelMatrix();
//	ofMatrix4x4 viewMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
//	ofMatrix4x4 projectionMatrix = ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
//	ofMatrix4x4 mvp = projectionMatrix * viewMatrix * modelMatrix;
//	shader.begin();
//	
//	shader.setUniformMatrix4f("modelViewProjectionMatrix", mvp);
//	shader.setUniformMatrix4f("modelMatrix", modelMatrix);
//	
//	shader.setUniform1f("brightness", 1.5f); // 1.5x brighter
//	shader.setUniform1f("specularStrength", 0.3f); // Specular amount
//	shader.setUniform1f("exposure", 2.0f); // Higher exposure
//	if(currentTexture == Texture::HDR_DAY){
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapDay.getTextureId());
//		shader.setUniform1i("environmentMap", 0);
//	}
//	else if(currentTexture == Texture::HDR_NIGHT){
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapNight.getTextureId());
//		shader.setUniform1i("environmentMap", 0);
//	}
//	model.draw(OF_MESH_FILL);
//	shader.end();
//	ofPopMatrix();
//	ofDisableDepthTest();
//}
