//
// Created by zackm on 2025-11-17.
//

//
// Material.h — version hybride Phong + PBR
//

#ifndef OF_CLION_MINIMAL_MATERIAL_H
#define OF_CLION_MINIMAL_MATERIAL_H

#pragma once
#include "ofMain.h"

class Material {
public:

    ofColor ambientColor;
    ofColor diffuseColor;
    ofColor emissiveColor;
    ofColor specularColor;
    float shininess;
    float metallic = 0.0f;
    float roughness = 1.0f;

    ofImage texture_diffuse;
    ofImage texture_metallic;
    ofImage texture_roughness;
    ofImage texture_occlusion;

    bool hasAOTexture = true;
    bool hasMetalnessTexture = true;

    Material()
        : ambientColor(ofColor::white)
        , diffuseColor(ofColor::white)
        , emissiveColor(ofColor::black)
        , specularColor(ofColor::white)
        , shininess(32.0f)
        , metallic(0.0f)
        , roughness(0.5f)
    {
        loadDiffuseTexture("textures/metal_plate_diffuse_1k.jpg");
        loadMetallicTexture("textures/metal_plate_metallic_1k.jpg");
        loadRoughnessTexture("textures/metal_plate_roughness_1k.jpg");
        loadOcclusionTexture("textures/metal_plate_ao_1k.jpg");
    }

    Material(
        const ofColor& ambient,
        const ofColor& diffuse,
        const ofColor& emissive,
        const ofColor& specular,
        float shininessValue,
        float metallicValue = 0.0f,
        float roughnessValue = 0.5f
    )
        : ambientColor(ambient)
        , diffuseColor(diffuse)
        , emissiveColor(emissive)
        , specularColor(specular)
        , shininess(shininessValue)
        , metallic(metallicValue)
        , roughness(roughnessValue)
    {
        loadDiffuseTexture("textures/metal_plate_diffuse_1k.jpg");
        loadMetallicTexture("textures/metal_plate_metallic_1k.jpg");
        loadRoughnessTexture("textures/metal_plate_roughness_1k.jpg");
        loadOcclusionTexture("textures/metal_plate_ao_1k.jpg");
    }

    // ---------------------------------------------------------
    // Utility texture loader (private helper)
    // ---------------------------------------------------------
private:
    void loadTexture(ofImage& img, const std::string& fileName) {
        if (fileName.empty()) {
            ofLogWarning("Material") << "Texture filename is empty, skipping load.";
            return;
        }

        if (!img.load(fileName)) {
            ofLogError("Material") << "Failed to load texture: " << fileName;
            return;
        }

        img.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    }

public:
    // ---------------------------------------------------------
    // Public texture setters
    // ---------------------------------------------------------
    void loadDiffuseTexture(const std::string& fileName) {
        loadTexture(texture_diffuse, fileName);
    }

    void loadMetallicTexture(const std::string& fileName) {
        loadTexture(texture_metallic, fileName);
    }

    void loadRoughnessTexture(const std::string& fileName) {
        loadTexture(texture_roughness, fileName);
    }

    void loadOcclusionTexture(const std::string& fileName) {
        loadTexture(texture_occlusion, fileName);
    }

    // ---------------------------------------------------------
    // Color / parameter setters
    // ---------------------------------------------------------
    void setAmbientColor(const ofColor& c)   { ambientColor = c; }
    void setDiffuseColor(const ofColor& c)   { diffuseColor = c; }
    void setEmissiveColor(const ofColor& c)  { emissiveColor = c; }
    void setSpecularColor(const ofColor& c)  { specularColor = c; }
    void setShininess(float s)               { shininess = s; }

    void setMetallic(float m) {
        if (hasMetalnessTexture) metallic = m;
    }

    void setRoughness(float r) {
        roughness = r;
    }

    void setHasMetalnessTexture(bool b) { hasMetalnessTexture = b; }
    void setHasAOTexture(bool b) { hasAOTexture = b; }
};

#endif // OF_CLION_MINIMAL_MATERIAL_H
