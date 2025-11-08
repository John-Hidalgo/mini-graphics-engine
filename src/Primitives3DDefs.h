#pragma once
#include "ofMain.h"
#include "BoundingBox.h"
#include "Canvas.h"
#include "Light.h"

enum class Primitive3DType { NONE, SPHERE, CUBE, CYLINDER, CONE, TORUS, PYRAMID };

struct Primitive3D {
	Primitive3DType type;
	ofPoint position;
	ofColor color;
	float size = 100.0f;
	ofMesh mesh;
	bool isSelected = false;
	ofShader shader_lambert;
	ofColor color_ambient;
	ofColor color_diffuse;
	BoundingBox bbox;

	// 7.2 Pour les material
	ofMaterial material;
	bool isMaterialActive = false;

	void setup() {
		shader_lambert.load("shaders/lambert_330_vs.glsl", "shaders/lambert_330_fs.glsl");
		color_ambient = ofColor(50, 50, 50);
		color_diffuse = ofColor(200, 200, 200);

		// 7.2 - Setup le material
		material.setAmbientColor(ofColor(63, 63, 63));
		material.setDiffuseColor(ofColor(127, 0, 0));
		material.setEmissiveColor(ofColor( 31, 0, 0));
		material.setSpecularColor(ofColor(127, 127, 127));
		material.setShininess(16.0f);
	}
	
	void draw(ofLight& canvasLight, bool showBoundingBox = false, const std::vector<LightData>& lights = {}) {
    ofEnableDepthTest();
    ofEnableLighting();

    shader_lambert.begin();

    shader_lambert.setUniformMatrix4f("modelViewMatrix", ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
    shader_lambert.setUniformMatrix4f("projectionMatrix", ofGetCurrentMatrix(OF_MATRIX_PROJECTION));

    shader_lambert.setUniform3f("color_ambient", color_ambient.r / 255.0f,
                                color_ambient.g / 255.0f,
                                color_ambient.b / 255.0f);
    shader_lambert.setUniform3f("color_diffuse", color_diffuse.r / 255.0f,
                                color_diffuse.g / 255.0f,
                                color_diffuse.b / 255.0f);
    shader_lambert.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
    shader_lambert.setUniform1f("brightness", 0.5f);

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
        shader_lambert.setUniform3f("light_position", canvasLight.getGlobalPosition());
    }

    ofPushMatrix();
    ofTranslate(position);
    ofSetColor(color);

	// 7.2 - Activer le material
	if(isMaterialActive){
		material.begin();
	}

    mesh.draw();

	// 7.2 - Désactiver le material
	if(isMaterialActive) {
		material.end();
	}

    if (showBoundingBox) {
        ofPushStyle();
        ofNoFill();
        ofSetColor(0, 255, 0);
        ofSetLineWidth(2);
        bbox.draw();
        ofPopStyle();
    }

    ofPopMatrix();

    shader_lambert.end();
    ofDisableLighting();
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
};
