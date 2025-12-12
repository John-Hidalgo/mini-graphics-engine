#version 330 core

uniform samplerCube environmentMap;
uniform vec3 cameraPos;
uniform float brightness;

in vec3 vWorldPos;
in vec3 vNormal;

out vec4 fragColor;

void main() {
	vec3 N = normalize(vNormal);
	vec3 V = normalize(cameraPos - vWorldPos);

	vec3 R = reflect(-V, N);
	R.y = -R.y;
	vec3 envColor = texture(environmentMap, R).rgb;
	float F0 = 0.04;
	float cosTheta = max(dot(V, N), 0.0);
	float fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

	vec3 finalColor = envColor * fresnel * brightness;
	fragColor = vec4(pow(finalColor, vec3(1.0/2.2)), 1.0);
}
