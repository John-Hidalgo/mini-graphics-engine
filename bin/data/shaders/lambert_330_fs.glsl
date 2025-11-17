#version 330

in vec3 surface_position;
in vec3 surface_normal;
out vec4 fragment_color;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;
uniform vec3 light_position;
uniform float u_time;
uniform float u_colorSpeed;
uniform bool u_animateColour;
uniform float mat_metallic;
uniform float mat_roughness;
uniform float mat_shininess;

void main()
{
    vec3 n = normalize(surface_normal);
    vec3 l = normalize(light_position - surface_position);
    float reflection_diffuse = max(dot(n, l), 0.0);

    vec3 v = normalize(-surface_position);
    vec3 r = reflect(-l, n);
    float reflection_specular = pow(max(dot(v, r), 0.0), brightness);

    vec3 finalAmbient = color_ambient;
    vec3 finalDiffuse = color_diffuse;

    if (u_animateColour) {
        float redParam = u_time * u_colorSpeed;
        float r = (sin(redParam) * 0.3 + 0.7);
        float greenParam = u_time * u_colorSpeed * 1.7 + 1.0;
        float g = (cos(greenParam) * 0.3 + 0.7);
        float blueParam = u_time * u_colorSpeed * 0.8 + 2.0;
        float b = (sin(blueParam * 2.0) * 0.4 + 0.6);
        finalAmbient *= vec3(r, g, b);
        finalDiffuse *= vec3(r, g, b);
    }

    // PBR simplifié
    vec3 diffuse = finalDiffuse * reflection_diffuse * (1.0 - mat_metallic);
    vec3 specular = color_specular * reflection_specular * (mat_metallic + 0.1);

    fragment_color = vec4(finalAmbient + diffuse + specular, 1.0);
}


