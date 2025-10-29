#version 330

in vec3 surface_color;
in vec3 surface_normal;
in vec3 surface_position;

out vec4 fragment_color;

uniform float outline_threshold;
uniform vec3 outline_color;
uniform float u_time;
uniform float u_colorSpeed;
uniform bool u_animateColour;

void main()
{
  vec3 finalColor = surface_color;
  
  if (u_animateColour)
  {
	float redParam = u_time * u_colorSpeed;
	float r = (sin(redParam) * 0.3 + 0.7);
	float greenParam = u_time * u_colorSpeed * 1.7 + 1.0;
	float g = (cos(greenParam) * 0.3 + 0.7);
	float blueParam = u_time * u_colorSpeed * 0.8 + 2.0;
	float b = (sin(blueParam * 2.0) * 0.4 + 0.6);
	finalColor *= vec3(r, g, b);
  }
  vec3 n = normalize(surface_normal);
  vec3 v = normalize(-surface_position);
  float edge = dot(n, v);
  
  if (edge < outline_threshold) {
	fragment_color = vec4(outline_color, 1.0);
  } else {
	fragment_color = vec4(finalColor, 1.0);
  }
}
