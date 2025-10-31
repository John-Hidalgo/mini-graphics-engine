#version 330 core

uniform samplerCube environmentMap;
uniform vec3 cameraPos;
uniform float brightness;
uniform float specularStrength;

in vec3 vWorldPos;
in vec3 vNormal;

out vec4 fragColor;

void main() {
	vec3 normal = normalize(vNormal);
	vec3 viewDir = normalize(cameraPos - vWorldPos);
	vec3 reflectDir = reflect(-viewDir, normal);
	reflectDir.y = -reflectDir.y;
	
	vec3 envColor = texture(environmentMap, reflectDir).rgb;
	
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * vec3(1.0);

	vec3 finalColor = envColor * brightness + specular;
	
	fragColor = vec4(finalColor, 1.0);
}
