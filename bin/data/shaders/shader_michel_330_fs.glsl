#version 330

uniform sampler2D tex;
uniform float ambient;

in vec2 vTexCoord;
out vec4 fragColor;

void main() {
	vec4 texColor = texture(tex, vTexCoord);
	fragColor = texColor * (ambient + 0.7);
}
