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
    float metallic;           // mat_metallic
    float roughness;          // mat_roughness

    Material()
        : ambientColor(ofColor::white)
        , diffuseColor(ofColor::white)
        , emissiveColor(ofColor::black)
        , specularColor(ofColor::white)
        , shininess(32.0f)
        , metallic(0.0f)
        , roughness(0.5f)
    {}

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
    {}

    // --- Setter helpers ---
    void setAmbientColor(const ofColor& c)   { ambientColor = c; }
    void setDiffuseColor(const ofColor& c)   { diffuseColor = c; }
    void setEmissiveColor(const ofColor& c)  { emissiveColor = c; }
    void setSpecularColor(const ofColor& c)  { specularColor = c; }
    void setShininess(float s)               { shininess = s; }
    void setMetallic(float m)                { metallic = m; }
    void setRoughness(float r)               { roughness = r; }

    // --- Appliquer au shader Lambert/Phong actuel ---
    void applyToShader(ofShader& shader) const
    {
        // PHONG COMPATIBLE UNIFORMS
        shader.setUniform3f("color_ambient",
            ambientColor.r / 255.0f,
            ambientColor.g / 255.0f,
            ambientColor.b / 255.0f
        );

        shader.setUniform3f("color_diffuse",
            diffuseColor.r / 255.0f,
            diffuseColor.g / 255.0f,
            diffuseColor.b / 255.0f
        );

        shader.setUniform3f("color_specular",
            specularColor.r / 255.0f,
            specularColor.g / 255.0f,
            specularColor.b / 255.0f
        );

        // brightness = shininess dans ton shader
        shader.setUniform1f("brightness", shininess);

        // PBR ADD-ON UNIFORMS
        shader.setUniform1f("mat_metallic",  metallic);
        shader.setUniform1f("mat_roughness", roughness);
    }
};

#endif //OF_CLION_MINIMAL_MATERIAL_H