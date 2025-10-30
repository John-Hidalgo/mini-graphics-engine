#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D normalMap;
uniform vec3 lightPosition;

in vec2 vTexCoord;
in vec3 vFragPos;
in mat3 TBN;

out vec4 fragColor;

void main() {
	vec3 color = texture(colorTexture, vTexCoord).rgb;

	vec3 normal = texture(normalMap, vTexCoord).rgb;
	normal = normalize((normal * 2.0 - 1.0) * vec3(1.0, 1.0, 1.0));
	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPosition - vFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = 2.0 * diff * color;
	vec3 ambient = 0.4 * color;

	vec3 viewDir = normalize(-vFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = spec * vec3(1.0);

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}
