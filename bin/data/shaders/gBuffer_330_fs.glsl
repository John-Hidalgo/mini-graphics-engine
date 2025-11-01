#version 330 core
in vec3 FragPos;
in vec3 Normal;

layout(location=0) out vec3 gPosition;
layout(location=1) out vec3 gNormal;
layout(location=2) out vec3 gAlbedo;

void main(){
	gPosition = FragPos;
	gNormal = normalize(Normal);
	gAlbedo = vec3(0.3,0.8,0.3);
}
