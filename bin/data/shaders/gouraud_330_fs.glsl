// IFT3100H25 ~ gouraud_330_fs.glsl

#version 330

// attribut interpolé à partir des valeurs en sortie du shader de sommet
in vec3 surface_color;

// attribut en sortie
out vec4 fragment_color;

uniform float u_time;
uniform float u_colorSpeed;
uniform bool u_animateColour;

void main()
{
  vec3 finalColor = surface_color;
  
  if (u_animateColour)
  {
	float redParam = u_time * u_colorSpeed;
	float r = surface_color.r * (sin(redParam) * 0.3 + 0.7);
	float greenParam = u_time * u_colorSpeed * 1.7 + 1.0;
	float g = surface_color.g * (cos(greenParam) * 0.3 + 0.7);
	float blueParam = u_time * u_colorSpeed * 0.8 + 2.0;
	float b = surface_color.b * (sin(blueParam * 2.0) * 0.4 + 0.6);
	finalColor = vec3(r, g, b);
  }

  // calculer la couleur du fragment
  fragment_color = vec4(finalColor, 1.0);
}
