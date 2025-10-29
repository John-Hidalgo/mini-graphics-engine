#version 330

in vec4 position;
in vec4 normal;

out vec3 surface_color;
out vec3 surface_normal;
out vec3 surface_position;

uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform vec3 light_position;
uniform int bands;
uniform float outline_threshold;
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

  mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));
  surface_normal = vec3(normalMatrix * normal);
  surface_position = vec3(modelViewMatrix * animatedPosition);
  
  vec3 n = normalize(surface_normal);
  vec3 l = normalize(light_position - surface_position);
  
  float reflection_diffuse = max(dot(n, l), 0.0);
  float cel_diffuse = floor(reflection_diffuse * float(bands)) / float(bands - 1);
  cel_diffuse = max(cel_diffuse, 0.4);
  
  float reflection_specular = 0.0;

  if (cel_diffuse > 0.0)
  {
	vec3 v = normalize(-surface_position);
	vec3 r = reflect(-l, n);
	reflection_specular = pow(max(dot(v, r), 0.0), 32.0);
	reflection_specular = step(0.5, reflection_specular);
  }
  
  surface_color = vec3(color_ambient + color_diffuse * cel_diffuse + color_specular * reflection_specular);
  gl_Position = projectionMatrix * modelViewMatrix * animatedPosition;
}
