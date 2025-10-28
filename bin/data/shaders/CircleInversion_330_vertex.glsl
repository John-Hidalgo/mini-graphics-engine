#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

out vec3 vPosition;
out vec2 vTexCoord;

uniform mat4 modelViewProjectionMatrix;

void main()
{
	vPosition = aPos;
	vTexCoord = aTexCoord;
	gl_Position = modelViewProjectionMatrix * vec4(aPos, 1.0);
}
