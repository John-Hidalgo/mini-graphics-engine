#version 330 core

in vec4 position;
in vec3 normal;
in vec2 texcoord;
in vec4 tangent;

uniform mat4 modelMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform float displacementScale;

uniform sampler2D displacementMap;

out vec2 vTexCoord;
out vec3 vFragPos;
out mat3 TBN;

void main() {
	vTexCoord = texcoord;

	float height = texture(displacementMap, texcoord).r;
	vec3 displacedPosition = vec3(position.xyz) + normal * height * displacementScale;

	vFragPos = vec3(modelMatrix * vec4(displacedPosition, 1.0));
	vec3 T = normalize(mat3(modelMatrix) * tangent.xyz);
	vec3 N = normalize(mat3(modelMatrix) * normal);
	vec3 B = cross(N, T) * tangent.w;
	TBN = mat3(T, B, N);

	gl_Position = modelViewProjectionMatrix * vec4(displacedPosition, 1.0);
}
