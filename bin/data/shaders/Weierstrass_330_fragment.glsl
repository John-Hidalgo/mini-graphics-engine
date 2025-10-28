#version 330 core

in vec3 vPosition;
in vec2 vTexCoord;
out vec4 FragColor;

uniform float u_time;
uniform vec2 u_resolution;

float weierstrass(float x, float time, int terms) {
	float sum = 0.0;
	float a = 0.5;
	float b = 3.0;
	
	for(int n = 0; n < terms; n++) {
		float nf = float(n);
		sum += pow(a, nf) * cos(pow(b, nf) * x * 3.14159 + time * (0.5 + 0.3 * sin(nf * 0.7)));
	}
	return sum;
}

void main()
{
	vec2 uv = vPosition.xz * 0.5;
	float time = u_time * 0.5;

	float wx = weierstrass(uv.x + sin(time * 0.3), time, 8);
	float wy = weierstrass(uv.y + cos(time * 0.4), time, 8);
	float wd = weierstrass(length(uv) * 2.0 + time * 0.2, time, 6);
	float wa = weierstrass(atan(uv.y, uv.x) * 4.0 + time, time, 5);
	
	float r = 0.5 + 0.5 * wx * wy;
	float g = 0.5 + 0.5 * wd * wa;
	float b = 0.5 + 0.5 * wx + wy * 0.5;

	vec3 color = vec3(
		r * (0.8 + 0.2 * sin(time)),
		g * (0.8 + 0.2 * cos(time * 1.3)),
		b * (0.8 + 0.2 * sin(time * 1.7))
	);
	color = color * color * 1.2;
	FragColor = vec4(color, 1.0);
}
