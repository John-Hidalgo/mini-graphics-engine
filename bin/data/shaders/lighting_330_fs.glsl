#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightPos;

float ShadowCalculation(vec3 fragPosWorld, vec3 normal) {
	vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPosWorld, 1.0);
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.005 * (1.0 - dot(normal, normalize(lightPos - fragPosWorld))), 0.001);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	if (projCoords.z > 1.0) shadow = 0.0;
	return shadow;
}

void main() {
	vec3 pos = texture(gPosition, TexCoords).rgb;
	vec3 norm = normalize(texture(gNormal, TexCoords).rgb);
	vec3 color = texture(gAlbedo, TexCoords).rgb;
	vec3 lightDir = normalize(lightPos - pos);
	float diff = max(dot(norm, lightDir), 0.0);
	float shadow = ShadowCalculation(pos, norm);
	vec3 lighting = (1.0 - shadow) * color * diff*2.0;

	FragColor = vec4(lighting, 1.0);
}
