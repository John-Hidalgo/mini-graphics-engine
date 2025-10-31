#version 330 core

layout(location = 0) in vec3 position;  // vec4 will NOT work here!
layout(location = 2) in vec3 normal;    // location 2, not 1

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

out vec3 vWorldPos;
out vec3 vNormal;

void main() {
	vWorldPos = (modelMatrix * vec4(position, 1.0)).xyz;
	vNormal = normalize(mat3(modelMatrix) * normal);
	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
