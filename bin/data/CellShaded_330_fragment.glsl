#version 330

in vec3 surface_color;
in vec3 surface_normal;
in vec3 surface_position;

out vec4 fragment_color;

uniform float outline_threshold;
uniform vec3 outline_color;

void main()
{
	fragment_color = vec4(surface_color, 1.0);
}
