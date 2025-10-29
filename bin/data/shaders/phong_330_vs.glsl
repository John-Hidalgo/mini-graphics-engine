// IFT3100H25 ~ phong_330_vs.glsl

#version 330

// attributs de sommet
in vec4 position;
in vec4 normal;

// attributs en sortie
out vec3 surface_position;
out vec3 surface_normal;

// attributs uniformes
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

uniform float u_time;
uniform float u_animationSpeed;
uniform float u_rippleAmplitude;
uniform bool u_animateSurface;

void main()
{
  vec4 animatedPosition = position;
  
  if (u_animateSurface)
  {
	float rippleParam = position.x * 5.0 + u_time * u_animationSpeed;
	float rippleHeight = sin(rippleParam) * u_rippleAmplitude;
	float waveParam = position.z * 3.0 + u_time * u_animationSpeed * 1.5;
	float waveOffset = cos(waveParam) * u_rippleAmplitude * 0.5;
	animatedPosition.y += rippleHeight;
	animatedPosition.x += waveOffset;
  }

  // calculer la matrice normale
  mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));

  // transformation de la normale du sommet dans l'espace de vue
  surface_normal = vec3(normalMatrix * normal);

  // transformation de la position du sommet dans l'espace de vue
  surface_position = vec3(modelViewMatrix * animatedPosition);

  // transformation de la position du sommet par les matrices de modèle, vue et projection
  gl_Position = projectionMatrix * modelViewMatrix * animatedPosition;
}
