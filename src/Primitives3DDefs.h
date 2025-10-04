#pragma once
#include "ofMain.h"

enum class Primitive3DType { NONE, SPHERE, CUBE, CYLINDER, CONE, TORUS, PYRAMID };

struct Primitive3D {
    Primitive3DType type;
    ofPoint position;
    ofColor color;
    float size = 100.0f;
    ofMesh mesh;
    bool isSelected = false;

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
        }

        // Face du dessous
        for (int i = 0; i < resolution; i++) {
            mesh.addVertex(bottomCenter);
            mesh.addVertex(bottomVertices[(i+1) % resolution]);
            mesh.addVertex(bottomVertices[i]);
        }

        // Les cotés / sides
        for (int i = 0; i < resolution; i++) {
            int next = (i+1) % resolution;

            mesh.addVertex(topVertices[i]);
            mesh.addVertex(bottomVertices[i]);
            mesh.addVertex(topVertices[next]);

            mesh.addVertex(topVertices[next]);
            mesh.addVertex(bottomVertices[i]);
            mesh.addVertex(bottomVertices[next]);
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
        }

        // Cotés du cone
        for (int i = 0; i < resolution; i++) {
            mesh.addVertex(apex);
            mesh.addVertex(baseVertices[i]);
            mesh.addVertex(baseVertices[(i+1) % resolution]);
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

                //Ajout de deux triangles
                mesh.addVertex(v1); mesh.addVertex(v2); mesh.addVertex(v3);
                mesh.addVertex(v1); mesh.addVertex(v3); mesh.addVertex(v4);
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
        mesh.addVertex(base1); mesh.addVertex(base2); mesh.addVertex(base3);
        mesh.addVertex(base1); mesh.addVertex(base3); mesh.addVertex(base4);

        // Cotés
        mesh.addVertex(apex); mesh.addVertex(base1); mesh.addVertex(base2);
        mesh.addVertex(apex); mesh.addVertex(base2); mesh.addVertex(base3);
        mesh.addVertex(apex); mesh.addVertex(base3); mesh.addVertex(base4);
        mesh.addVertex(apex); mesh.addVertex(base4); mesh.addVertex(base1);
    }
};