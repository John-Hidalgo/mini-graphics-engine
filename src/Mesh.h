#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

class Mesh {
public:
	GLuint VAO, VBO;
	GLsizei vertexCount;

	Mesh() : VAO(0), VBO(0), vertexCount(0) {}

	void Draw();
	static Mesh Cube(float size = 1.0f);
};
