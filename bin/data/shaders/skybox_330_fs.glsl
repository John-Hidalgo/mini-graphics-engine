#version 330 core

in vec3 texCoord;
out vec4 fragColor;

float hash(float n) {
	return fract(sin(n) * 43758.5453);
}

float noise(vec3 x) {
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f * f * (3.0 - 2.0 * f);
	
	float n = p.x + p.y * 157.0 + 113.0 * p.z;
	return mix(mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
				   mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
			   mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
				   mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
}

void main() {
	vec3 dir = normalize(texCoord);

	float gradient = smoothstep(-0.5, 0.8, normalize(texCoord).y);
	vec3 skyColor = mix(vec3(0.05, 0.05, 0.15),vec3(0.3, 0.4, 0.8),gradient);
	
	float stars = 0.0;

	for(int i = 0; i < 3; i++) {
		float scale = pow(2.0, float(i));
		float starValue = noise(dir * 200.0 * scale);
		
		if(starValue > 0.99 - (float(i) * 0.1)) {
			stars += (1.0 - float(i) * 0.3);
		}
	}
	
	float twinkle = sin(texCoord.x * 100.0 + texCoord.y * 70.0) * 0.3 + 0.7;
	stars *= twinkle;
	skyColor += stars * vec3(1.0, 1.0, 1.0);
	
	vec3 sunDir = normalize(vec3(0.3, 0.6, 0.2));
	float sun = max(0.0, dot(dir, sunDir));
	sun = smoothstep(0.998, 0.999, sun);
	skyColor = mix(skyColor, vec3(1.0, 0.95, 0.8), sun);
	float milkyWay = abs(dir.y) * 0.3;
	milkyWay = 1.0 - smoothstep(0.1, 0.3, milkyWay);
	skyColor += vec3(0.1, 0.08, 0.15) * milkyWay * 0.3;
	
	fragColor = vec4(skyColor, 1.0);
}
