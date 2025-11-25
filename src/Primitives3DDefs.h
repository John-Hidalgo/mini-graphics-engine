#pragma once
#include "ofMain.h"
#include "BoundingBox.h"
#include "Canvas.h"
#include "Light.h"
#include "Material.h"

enum class Primitive3DType { NONE, SPHERE, CUBE, CYLINDER, CONE, TORUS, PYRAMID, BEZIER_SURFACE };

struct Primitive3D {
	Primitive3DType type;
	ofPoint position;
	ofColor color;
	float size = 100.0f;
	ofMesh mesh;
	bool isSelected = false;
	ofShader shader;
	ofColor color_ambient;
	ofColor color_diffuse;
	BoundingBox bbox;

	// 7.2 Pour les material
	//ofMaterial material;
	Material material;
	bool isMaterialActive = false;

	// Pour les surfaces paramétriques
	std::vector<std::vector<glm::vec3>> controlPoints; // Grille 4x4 de points de contrôle
	int surfaceResolution = 20; // Résolution de la surface

	void setup() {
		shader.load("shaders/pbr_330_vs.glsl", "shaders/pbr_330_fs.glsl");
		color_ambient = ofColor(50, 50, 50);
		color_diffuse = ofColor(200, 200, 200);

		// 7.2 - Setup le material
		//material.setAmbientColor(ofColor(63, 63, 63));
		//material.setDiffuseColor(ofColor(127, 0, 0));
		//material.setEmissiveColor(ofColor( 31, 0, 0));
		//material.setSpecularColor(ofColor(127, 127, 127));
		//material.setShininess(16.0f);

		// Initialiser les points de contrôle pour Bézier si nécessaire
		if (type == Primitive3DType::BEZIER_SURFACE && controlPoints.empty()) {
			setupBezierControlPoints();
		}
	}

	void draw(ofLight& canvasLight, const glm::vec3 cameraPosition, bool showBoundingBox = false, const std::vector<LightData>& lights = {})
{
    ofEnableDepthTest();
    shader.begin();

    // --- Matrices ---
    glm::vec3 pos3(position.x, position.y, position.z);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos3);
    glm::mat4 view = ofGetCurrentViewMatrix();
    glm::mat4 proj = ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
		glm::mat4 modelView = view * model;

    shader.setUniformMatrix4f("modelViewMatrix", modelView);
    shader.setUniformMatrix4f("projectionMatrix", proj);

    // --- Lights ---
    glm::vec3 lightPos;
    glm::vec3 lightColor;

    if (!lights.empty()) {
        glm::vec3 combinedPos(0.0f);
        glm::vec3 combinedColor(0.0f);
        float totalAtt = 0.0f;

        for (auto& l : lights) {
            float w = l.attenuation;
            combinedPos += glm::vec3(l.position) * w;
            combinedColor += glm::vec3(
                l.color.r / 255.f,
                l.color.g / 255.f,
                l.color.b / 255.f
            ) * w;
            totalAtt += w;
        }
        if (totalAtt == 0.0f) totalAtt = 1.0f;

        lightPos   = combinedPos / totalAtt;
        lightColor = combinedColor / totalAtt;
    }
    else {
        lightPos = canvasLight.getGlobalPosition();
        lightColor = glm::vec3(
            canvasLight.getDiffuseColor().r / 255.f,
            canvasLight.getDiffuseColor().g / 255.f,
            canvasLight.getDiffuseColor().b / 255.f
        );
    }

    shader.setUniform3f("light_position", lightPos);
    shader.setUniform3f("light_color", lightColor);
		shader.setUniform1f("light_intensity", 1.0f);

    // --- Material ---
    float metallic;
    float roughness;
		float shininess;

    if (isMaterialActive) {
        metallic  = material.metallic;
        roughness = material.roughness;
    	shininess = material.shininess;
    }
    else {
        metallic = 0.0f;
        roughness = 1.0f;
    	shininess = 0.0f;
    }
		shader.setUniform3f("material_color_ambient", material.ambientColor.r/255.0f, material.ambientColor.g/255.0f, material.ambientColor.b/255.0f);
		shader.setUniform3f("material_color_diffuse", material.diffuseColor.r/255.0f, material.diffuseColor.g/255.0f, material.diffuseColor.b/255.0f);
		shader.setUniform3f("material_color_specular", material.specularColor.r/255.0f, material.specularColor.g/255.0f, material.specularColor.b/255.0f);
    shader.setUniform1f("material_brightness", shininess);
    shader.setUniform1f("material_metallic", metallic);
    shader.setUniform1f("material_roughness", roughness);
		shader.setUniform3f("material_fresnel_ior", glm::vec3(0.04f, 0.04f, 0.04f));
		shader.setUniform1f("tone_mapping_exposure", 1.0f);
		shader.setUniform1i("tone_mapping_toggle", true);
		shader.setUniform1f("material_occlusion", 1.0f);


    // --- Draw mesh (NO OF TRANSFORM!) ---
    mesh.draw();

    // --- Debug bounding box ---
    if (showBoundingBox) {
        ofPushStyle();
        ofNoFill();
        ofSetColor(0, 255, 0);
        bbox.draw();
        ofPopStyle();
    }

    shader.end();
    ofDisableDepthTest();
}


	void generateMesh() {
		mesh.clear();
		switch(type) {
			case Primitive3DType::SPHERE:
				generateSphere();
				break;
			case Primitive3DType::CUBE:
				generateCube();
				break;
			case Primitive3DType::CYLINDER:
				generateCylinder();
				break;
			case Primitive3DType::CONE:
				generateCone();
				break;
			case Primitive3DType::TORUS:
				generateTorus();
				break;
			case Primitive3DType::PYRAMID:
				generatePyramid();
				break;
			case Primitive3DType::BEZIER_SURFACE:
				generateBezierSurface();
				break;
			default:
				break;
		}
		bbox.reset();
		for (auto &v : mesh.getVertices()) {
			bbox.expandToInclude(v);
		}
	}

	// Pour calculer la distance entre les points du debut du mousePressed et a la finn du mouseReleased
	void setSizeFromPoints(const ofPoint& start, const ofPoint& end) {
		float distance = start.distance(end);
		size = distance;

		// On redessine avec la nouvelle dimension
		generateMesh();
	}

	// 8.3 Pour les surfaces paramétriques
	// Méthodes pour la surface de Bézier
	void setupBezierControlPoints() {
		controlPoints.clear();
		controlPoints.resize(4);

		float scale = size * 0.5f;

		// Initialiser une grille 4x4 de points de contrôle pour une surface de Bézier bicubique
		for (int i = 0; i < 4; i++) {
			controlPoints[i].resize(4);
			for (int j = 0; j < 4; j++) {
				float x = (i - 1.5f) * scale;
				float z = (j - 1.5f) * scale;
				float y = 0.0f;

				// Créer une forme intéressante (ex: vague)
				if ((i == 1 || i == 2) && (j == 1 || j == 2)) {
					y = scale * 0.5f;
				}
				if (i == 1 && j == 1) {
					y = scale * 0.8f;
				}

				controlPoints[i][j] = glm::vec3(x, y, z);
			}
		}
	}

	void updateBezierControlPoint(int i, int j, const glm::vec3& newPos) {
		if (i >= 0 && i < 4 && j >= 0 && j < 4) {
			controlPoints[i][j] = newPos;
			generateMesh(); // Régénérer le maillage
		}
	}

	void setBezierPreset(int preset) {
		ofLogNotice("Primitive3D") << "Setting Bezier preset: " << preset;

		if (controlPoints.empty()) {
			controlPoints.resize(4);
			for (int i = 0; i < 4; i++) {
				controlPoints[i].resize(4);
			}
		}

		switch(preset) {
			case 0: // Plat
				applyFlatPreset();
				break;

			case 1: // Colline
				applyHillPreset();
				break;

			case 2: // Vallée
				applyValleyPreset();
				break;

			case 3: // Vague
				applyWavePreset();
				break;

			case 4: // Selle
				applySaddlePreset();
				break;

			case 5: // Torsade
				applyTwistPreset();
				break;
		}

		generateMesh();
	}

private:
	void generateSphere(int resolution = 24) {
		mesh = ofMesh::sphere(size, resolution);
	}

	void generateCube() {
		mesh = ofMesh::box(size, size, size);
	}

	void generateCylinder(int resolution = 24) {
		float radius = size * 0.5f;
		float height = size;
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);

		// Vertex des faces en cercle au dessus et dessous
		std::vector<glm::vec3> topVertices, bottomVertices;
		for (int i = 0; i < resolution; i++) {
			float angle = TWO_PI * i / resolution;
			float x = cos(angle) * radius;
			float z = sin(angle) * radius;

			topVertices.push_back(glm::vec3(x, height/2, z));
			bottomVertices.push_back(glm::vec3(x, -height/2, z));
		}

		// Points centraux des faces en cercles dessus / dessous
		glm::vec3 topCenter(0, height/2, 0);
		glm::vec3 bottomCenter(0, -height/2, 0);

		// Face du dessus
		for (int i = 0; i < resolution; i++) {
			mesh.addVertex(topCenter);
			mesh.addVertex(topVertices[i]);
			mesh.addVertex(topVertices[(i+1) % resolution]);
			glm::vec3 normal(0, 1, 0);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
		}

		// Face du dessous
		for (int i = 0; i < resolution; i++) {
			mesh.addVertex(bottomCenter);
			mesh.addVertex(bottomVertices[(i+1) % resolution]);
			mesh.addVertex(bottomVertices[i]);
			glm::vec3 normal(0, -1, 0);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
		}

		// Les cotés / sides
		for (int i = 0; i < resolution; i++) {
			int next = (i+1) % resolution;

			mesh.addVertex(topVertices[i]);
			mesh.addVertex(bottomVertices[i]);
			mesh.addVertex(topVertices[next]);
			glm::vec3 v1 = topVertices[i];
			glm::vec3 v2 = bottomVertices[i];
			glm::vec3 v3 = topVertices[next];
			glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
			mesh.addNormal(normal);
			mesh.addNormal(normal);
			mesh.addNormal(normal);

			mesh.addVertex(topVertices[next]);
			mesh.addVertex(bottomVertices[i]);
			mesh.addVertex(bottomVertices[next]);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
		}
	}

	void generateCone(int resolution = 24) {
		float radius = size * 0.5f;
		float height = size;
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);

		// Vertex de la base du cone
		std::vector<glm::vec3> baseVertices;
		glm::vec3 apex(0, height/2, 0);
		glm::vec3 baseCenter(0, -height/2, 0);

		for (int i = 0; i < resolution; i++) {
			float angle = TWO_PI * i / resolution;
			float x = cos(angle) * radius;
			float z = sin(angle) * radius;
			baseVertices.push_back(glm::vec3(x, -height/2, z));
		}

		// Base
		for (int i = 0; i < resolution; i++) {
			mesh.addVertex(baseCenter);
			mesh.addVertex(baseVertices[(i+1) % resolution]);
			mesh.addVertex(baseVertices[i]);
			glm::vec3 normal(0, -1, 0);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
		}

		// Cotés du cone
		for (int i = 0; i < resolution; i++) {
			mesh.addVertex(apex);
			mesh.addVertex(baseVertices[i]);
			mesh.addVertex(baseVertices[(i+1) % resolution]);
			glm::vec3 edge1 = baseVertices[i] - apex;
			glm::vec3 edge2 = baseVertices[(i+1) % resolution] - apex;
			glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
			mesh.addNormal(normal);
			mesh.addNormal(normal);
			mesh.addNormal(normal);
		}
	}

	void generateTorus(int tubeResolution = 12, int circleResolution = 24) {
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);

		float circleRadius = size * 0.5f;
		float tubeRadius = size * 0.2f;

		for (int i = 0; i < circleResolution; i++) {
			float circleAngle1 = TWO_PI * i / circleResolution;
			float circleAngle2 = TWO_PI * (i + 1) / circleResolution;

			for (int j = 0; j < tubeResolution; j++) {
				float tubeAngle1 = TWO_PI * j / tubeResolution;
				float tubeAngle2 = TWO_PI * (j + 1) / tubeResolution;

				// Calcul des vertex du donet
				glm::vec3 v1 = getTorusVertex(circleAngle1, tubeAngle1, circleRadius, tubeRadius);
				glm::vec3 v2 = getTorusVertex(circleAngle2, tubeAngle1, circleRadius, tubeRadius);
				glm::vec3 v3 = getTorusVertex(circleAngle2, tubeAngle2, circleRadius, tubeRadius);
				glm::vec3 v4 = getTorusVertex(circleAngle1, tubeAngle2, circleRadius, tubeRadius);
				glm::vec3 normal1 = glm::normalize(v1 - glm::vec3(cos(circleAngle1)*circleRadius, 0, sin(circleAngle1)*circleRadius));
				glm::vec3 normal2 = glm::normalize(v2 - glm::vec3(cos(circleAngle2)*circleRadius, 0, sin(circleAngle2)*circleRadius));
				glm::vec3 normal3 = glm::normalize(v3 - glm::vec3(cos(circleAngle2)*circleRadius, 0, sin(circleAngle2)*circleRadius));
				glm::vec3 normal4 = glm::normalize(v4 - glm::vec3(cos(circleAngle1)*circleRadius, 0, sin(circleAngle1)*circleRadius));

				//Ajout de deux triangles
				mesh.addVertex(v1); mesh.addVertex(v2); mesh.addVertex(v3);
				mesh.addNormal(normal1);mesh.addNormal(normal2);mesh.addNormal(normal3);
				mesh.addVertex(v1); mesh.addVertex(v3); mesh.addVertex(v4);
				mesh.addNormal(normal1);mesh.addNormal(normal3);mesh.addNormal(normal4);
			}
		}
	}

	glm::vec3 getTorusVertex(float circleAngle, float tubeAngle, float circleRadius, float tubeRadius) {
		return glm::vec3(
				(circleRadius + tubeRadius * cos(tubeAngle)) * cos(circleAngle),
				tubeRadius * sin(tubeAngle),
				(circleRadius + tubeRadius * cos(tubeAngle)) * sin(circleAngle)
		);
	}

	void generatePyramid() {
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);

		float baseSize = size;
		float height = size;
		float halfBase = baseSize * 0.5f;
		glm::vec3 apex(0, height/2, 0);
		glm::vec3 base1(-halfBase, -height/2, -halfBase);
		glm::vec3 base2(halfBase, -height/2, -halfBase);
		glm::vec3 base3(halfBase, -height/2, halfBase);
		glm::vec3 base4(-halfBase, -height/2, halfBase);

		// Base faites de deux triangles
		mesh.addVertex(base1); mesh.addNormal(glm::vec3(0, -1, 0));
		mesh.addVertex(base2); mesh.addNormal(glm::vec3(0, -1, 0));
		mesh.addVertex(base3); mesh.addNormal(glm::vec3(0, -1, 0));

		mesh.addVertex(base1); mesh.addNormal(glm::vec3(0, -1, 0));
		mesh.addVertex(base3); mesh.addNormal(glm::vec3(0, -1, 0));
		mesh.addVertex(base4); mesh.addNormal(glm::vec3(0, -1, 0));

		// Cotés
		glm::vec3 frontNormal = glm::normalize(glm::cross(base2 - apex, base1 - apex));
		mesh.addVertex(apex); mesh.addNormal(frontNormal);
		mesh.addVertex(base1); mesh.addNormal(frontNormal);
		mesh.addVertex(base2); mesh.addNormal(frontNormal);

		glm::vec3 rightNormal = glm::normalize(glm::cross(base3 - apex, base2 - apex));
		mesh.addVertex(apex); mesh.addNormal(rightNormal);
		mesh.addVertex(base2); mesh.addNormal(rightNormal);
		mesh.addVertex(base3); mesh.addNormal(rightNormal);

		glm::vec3 backNormal = glm::normalize(glm::cross(base4 - apex, base3 - apex));
		mesh.addVertex(apex); mesh.addNormal(backNormal);
		mesh.addVertex(base3); mesh.addNormal(backNormal);
		mesh.addVertex(base4); mesh.addNormal(backNormal);

		glm::vec3 leftNormal = glm::normalize(glm::cross(base1 - apex, base4 - apex));
		mesh.addVertex(apex); mesh.addNormal(leftNormal);
		mesh.addVertex(base4); mesh.addNormal(leftNormal);
		mesh.addVertex(base1); mesh.addNormal(leftNormal);
	}

    // 8.3 Pour les surfaces paramétriques
	void generateBezierSurface() {
		mesh.clear();
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);

		if (controlPoints.empty()) {
			setBezierPreset(0);
		}

		// Générer les vertices
		int vertexCount = 0;
		for (int i = 0; i <= surfaceResolution; i++) {
			float u = (float)i / (float)surfaceResolution;

			for (int j = 0; j <= surfaceResolution; j++) {
				float v = (float)j / (float)surfaceResolution;

				glm::vec3 point = evaluateBezierSurface(u, v);
				mesh.addVertex(point);

				glm::vec3 normal = calculateBezierNormal(u, v);
				mesh.addNormal(normal);
				vertexCount++;
			}
		}

		// Générer les indices
		int triangleCount = 0;
		for (int i = 0; i < surfaceResolution; i++) {
			for (int j = 0; j < surfaceResolution; j++) {
				int v0 = i * (surfaceResolution + 1) + j;
				int v1 = v0 + 1;
				int v2 = (i + 1) * (surfaceResolution + 1) + j;
				int v3 = v2 + 1;

				mesh.addIndex(v0);
				mesh.addIndex(v2);
				mesh.addIndex(v1);

				mesh.addIndex(v1);
				mesh.addIndex(v2);
				mesh.addIndex(v3);

				triangleCount += 2;
			}
		}
	}

	// On fait l'évaluation d'un point sur la surface de Bézier bicubique
	glm::vec3 evaluateBezierSurface(float u, float v) {
		glm::vec3 result(0, 0, 0);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float basisI = bernsteinBasis(i, u);
				float basisJ = bernsteinBasis(j, v);
				result += controlPoints[i][j] * basisI * basisJ;
			}
		}

		return result;
	}

	// Calcul de la base de Bernstein
	float bernsteinBasis(int i, float t) {
		switch(i) {
			case 0: return (1 - t) * (1 - t) * (1 - t);
			case 1: return 3 * t * (1 - t) * (1 - t);
			case 2: return 3 * t * t * (1 - t);
			case 3: return t * t * t;
			default: return 0.0f;
		}
	}

	// Calcul de la normale à la surface
	glm::vec3 calculateBezierNormal(float u, float v) {
		// Calculer des dérivees partielles
		glm::vec3 du = evaluateBezierSurfaceDerivativeU(u, v);
		glm::vec3 dv = evaluateBezierSurfaceDerivativeV(u, v);

		// La normale = le produit vectoriel des dérivées
		return glm::normalize(glm::cross(du, dv));
	}

	// Dérivée par rapport à u
	glm::vec3 evaluateBezierSurfaceDerivativeU(float u, float v) {
		glm::vec3 result(0, 0, 0);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float basisDerivI = bernsteinBasisDerivative(i, u);
				float basisJ = bernsteinBasis(j, v);
				result += controlPoints[i][j] * basisDerivI * basisJ;
			}
		}

		return result;
	}

	// Dérivée par rapport à v
	glm::vec3 evaluateBezierSurfaceDerivativeV(float u, float v) {
		glm::vec3 result(0, 0, 0);

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float basisI = bernsteinBasis(i, u);
				float basisDerivJ = bernsteinBasisDerivative(j, v);
				result += controlPoints[i][j] * basisI * basisDerivJ;
			}
		}

		return result;
	}

	// Dérivée selon la base de Bernstein
	float bernsteinBasisDerivative(int i, float t) {
		switch(i) {
			case 0: return -3 * (1 - t) * (1 - t);
			case 1: return 3 * (1 - t) * (1 - t) - 6 * t * (1 - t);
			case 2: return 6 * t * (1 - t) - 3 * t * t;
			case 3: return 3 * t * t;
			default: return 0.0f;
		}
	}

	void applyFlatPreset() {
		// Plat
		float scale = size * 0.5f;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float x = (i - 1.5f) * scale;
				float z = (j - 1.5f) * scale;
				controlPoints[i][j] = glm::vec3(x, 0.0f, z);
			}
		}
	}

	void applyHillPreset() {
		float scale = size * 0.5f;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float x = (i - 1.5f) * scale;
				float z = (j - 1.5f) * scale;
				float y = 0.0f;

				// Crwer une colline au centre
				float distFromCenter = sqrt(x*x + z*z) / (scale * 1.5f);
				if (distFromCenter < 1.0f) {
					y = scale * 0.8f * (1.0f - distFromCenter * distFromCenter);
				}

				controlPoints[i][j] = glm::vec3(x, y, z);
			}
		}
	}

	void applyValleyPreset() {
		float scale = size * 0.5f;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float x = (i - 1.5f) * scale;
				float z = (j - 1.5f) * scale;
				float y = 0.0f;

				// Creer une valley en diagonal
				float diagonal = (x + z) / (scale * 2.0f);
				y = scale * 0.5f * (1.0f - abs(diagonal));

				controlPoints[i][j] = glm::vec3(x, y, z);
			}
		}
	}

	void applyWavePreset() {
		float scale = size * 0.5f;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float x = (i - 1.5f) * scale;
				float z = (j - 1.5f) * scale;

				// Créer une forme de sine-wave
				float wave = sin(x * 2.0f / scale) * cos(z * 2.0f / scale);
				float y = scale * 0.4f * wave;

				controlPoints[i][j] = glm::vec3(x, y, z);
			}
		}
	}

	void applySaddlePreset() {
		float scale = size * 0.5f;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float x = (i - 1.5f) * scale;
				float z = (j - 1.5f) * scale;

				// Creer une forme de selle / pringles
				float y = (x*x - z*z) / (scale * 2.0f);

				controlPoints[i][j] = glm::vec3(x, y, z);
			}
		}
	}

	void applyTwistPreset() {
		float scale = size * 0.5f;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float x = (i - 1.5f) * scale;
				float z = (j - 1.5f) * scale;

				// Creer une forme de torsade
				float angle = atan2(z, x);
				float y = scale * 0.3f * sin(angle * 2.0f);

				controlPoints[i][j] = glm::vec3(x, y, z);
			}
		}
	}
};
