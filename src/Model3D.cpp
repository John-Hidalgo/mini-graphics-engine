#include "Model3D.h"

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
	currentTexture = ProceduralTexture::NONE;
	previousLighting = Lighting::LAMBERT;
	celBands.set("Color Bands", 3, 2, 6);
	celSpecularSize.set("Specular Size", 0.3, 0.5, 1.0);
	celOutlineWidth.set("Outline Threshold", 0.1, 0.0, 1.0);
	celOutlineColor.set("Outline Color", ofColor(0, 0, 0));
	animationSpeed = 1.0f;
	rippleAmplitude = 0.1f;
	colorSpeed = 1.0f;
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
	currentTexture = ProceduralTexture::NONE;
	
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

void Model3D::setProceduralTexture(ProceduralTexture texture)
{
	if (texture == currentTexture && texture != ProceduralTexture::NONE) {
		toggleProceduralTexture(texture);
		return;
	}
	if (texture != ProceduralTexture::NONE && currentTexture == ProceduralTexture::NONE) {
		previousLighting = currentLighting;
	}
	
	currentTexture = texture;
	
	switch (texture) {
		case ProceduralTexture::INVERSION:
			shader = shader_inversion;
			ofLogNotice() << "Applied Circle Inversion texture over " << static_cast<int>(previousLighting);
			break;
		case ProceduralTexture::WEIERSTRASS:
			shader = shader_weierstrass;
			ofLogNotice() << "Applied Weierstrass texture over " << static_cast<int>(previousLighting);
			break;
		case ProceduralTexture::NONE:
			setShader(previousLighting);
			ofLogNotice() << "Removed procedural texture, restored to " << static_cast<int>(previousLighting);
			break;
	}
}

void Model3D::toggleProceduralTexture(ProceduralTexture texture)
{
	currentTexture == texture?
		setProceduralTexture(ProceduralTexture::NONE):
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
		if (model.hasMeshes()) {
			//ofLogNotice() << "Model has " << model.getNumMeshes() << " meshes";
			bbox.reset();
			for (int i = 0; i < model.getNumMeshes(); ++i) {
				const ofMesh& mesh = model.getMesh(i);

				for (auto& v : mesh.getVertices()) {
					ofVec3f worldV = v * model.getScale();
					bbox.expandToInclude(worldV);
				}
			}
			//ofLogNotice() << "BoundingBox min: " << bbox.min << " max: " << bbox.max;
		}
		else {
			ofLogError() << "Model has no meshes after loading!";
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
void Model3D::draw()
{
	ofSetBackgroundColor(color_background.r, color_background.g, color_background.b);
	ofEnableDepthTest();

	if (currentTexture == ProceduralTexture::NONE) {
		ofEnableLighting();
		light.enable();
	}

	shader.begin();
	shader.setUniformMatrix4f("modelViewMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
	shader.setUniformMatrix4f("projectionMatrix", ofGetCurrentMatrix(OF_MATRIX_PROJECTION));
	if (currentTexture == ProceduralTexture::NONE) {
		shader.setUniform1f("u_time", ofGetElapsedTimef());
		shader.setUniform1f("u_animationSpeed", animationSpeed);
		shader.setUniform1f("u_rippleAmplitude", rippleAmplitude);
		shader.setUniform1f("u_colorSpeed", colorSpeed);
		shader.setUniform1i("u_animateSurface", animateSurface);
		shader.setUniform1i("u_animateColour", animateColour);
		shader.setUniform3f("color_ambient", color_ambient.r / 255.0f, color_ambient.g / 255.0f, color_ambient.b / 255.0f);
		shader.setUniform3f("color_diffuse", color_diffuse.r / 255.0f, color_diffuse.g / 255.0f, color_diffuse.b / 255.0f);
		shader.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader.setUniform1f("brightness", 0.5f);
		shader.setUniform3f("light_position", light.getGlobalPosition());
		if(shader == shader_cell) {
			shader.setUniform1i("bands", celBands);
			shader.setUniform1f("outline_threshold", celOutlineWidth);
			shader.setUniform3f("outline_color",celOutlineColor->r / 255.0f, celOutlineColor->g / 255.0f, celOutlineColor->b / 255.0f);
			shader.setUniform3f("color_specular", 1.0f, 1.0f, 1.0f);
		}
	}
	else {
		shader.setUniform1f("u_time", ofGetElapsedTimef());
		shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
	}
	model.draw(OF_MESH_FILL);
	drawBoundingBox();
	shader.end();

	if (currentTexture == ProceduralTexture::NONE) {
		light.disable();
		ofDisableLighting();
	}
	ofDisableDepthTest();
}
