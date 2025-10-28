#version 330 core

in vec3 vPosition;
in vec2 vTexCoord;
out vec4 FragColor;

uniform float u_time;

vec2 circleInversion(vec2 z, vec2 center, float radius) {
	vec2 relative = z - center;
	float dist2 = dot(relative, relative);
	if(dist2 > 0.0001) {
		return center + (radius * radius / dist2) * relative;
	}
	return z;
}

void main()
{
	vec2 uv = (vTexCoord - 0.5) * 6.0;
	
	vec2 z = uv;
	float timeScale = 0.3;
	for(int i = 0; i < 8; i++) {
		float angle = float(i) * 3.14159 / 4.0 + u_time * timeScale;
		float radius = 0.4 + 0.1 * sin(u_time * 0.7 + float(i));
		vec2 center = vec2(cos(angle), sin(angle)) * radius;
		z = circleInversion(z, center, 0.6 + 0.2 * cos(u_time * 0.5 + float(i)));
	}
	
	float dist = length(z);
	float angle = atan(z.y, z.x);
	
	float r = 0.5 + 0.5 * sin(dist * 12.0 + u_time * 2.0);
	float g = 0.5 + 0.5 * cos(angle * 10.0 + u_time * 1.5);
	float b = 0.5 + 0.5 * sin(dist * 18.0 - angle * 8.0 + u_time * 2.5);
	
	vec3 color = vec3(r, g, b);
	color = color * color;
	FragColor = vec4(color, 1.0);
}
