// IFT3100H25 ~ blinn_phong_330_fs.glsl

#version 330

// attributs interpolés à partir des valeurs en sortie du shader de sommet
in vec3 surface_position;
in vec3 surface_normal;

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

// facteur de brillance spéculaire du matériau
uniform float brightness;

// position d'une source de lumière
uniform vec3 light_position;

uniform float u_time;
uniform float u_colorSpeed;
uniform bool u_animateColour;

void main()
{
  // re-normaliser la normale après interpolation
  vec3 n = normalize(surface_normal);

  // calculer la direction de la surface vers la lumière (l)
  vec3 l = normalize(light_position - surface_position);

  // calculer le niveau de réflexion diffuse (n • l)
  float reflection_diffuse = max(dot(n, l), 0.0);

  // réflexion spéculaire par défaut
  float reflection_specular = 0.0;

  // calculer la réflexion spéculaire seulement s'il y a réflexion diffuse
  if (reflection_diffuse > 0.0)
  {
	// calculer la direction de la surface vers la caméra (v)
	vec3 v = normalize(-surface_position);

	// calculer la direction du demi-vecteur de réflection (h) en fonction du vecteur de vue (v) et de lumière (l)
	vec3 h = normalize(v + l);

	// calculer le niveau de réflexion spéculaire (n • h)
	reflection_specular = pow(max(dot(n, h), 0.0), brightness);
  }

  vec3 finalAmbient = color_ambient;
  vec3 finalDiffuse = color_diffuse;
  
  if (u_animateColour)
  {
	float redParam = u_time * u_colorSpeed;
	float r = (sin(redParam) * 0.3 + 0.7);
	float greenParam = u_time * u_colorSpeed * 1.7 + 1.0;
	float g = (cos(greenParam) * 0.3 + 0.7);
	float blueParam = u_time * u_colorSpeed * 0.8 + 2.0;
	float b = (sin(blueParam * 2.0) * 0.4 + 0.6);

	finalAmbient *= vec3(r, g, b);
	finalDiffuse *= vec3(r, g, b);
  }

  // calculer la couleur du fragment
  fragment_color = vec4(
	finalAmbient +
	finalDiffuse * reflection_diffuse +
	color_specular * reflection_specular, 1.0);
}
