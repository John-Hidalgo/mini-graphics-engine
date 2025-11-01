#include "Mesh.h"

void Mesh::Draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}

Mesh Mesh::Cube(float size) {
	Mesh mesh;
	float s = size / 2.0f;
	
	float vertices[] = {
		// positions          // normals
		// Back face
		-s, -s, -s,  0.0f,  0.0f, -1.0f,
		 s, -s, -s,  0.0f,  0.0f, -1.0f,
		 s,  s, -s,  0.0f,  0.0f, -1.0f,
		 s,  s, -s,  0.0f,  0.0f, -1.0f,
		-s,  s, -s,  0.0f,  0.0f, -1.0f,
		-s, -s, -s,  0.0f,  0.0f, -1.0f,

		// Front face
		-s, -s,  s,  0.0f,  0.0f,  1.0f,
		 s,  s,  s,  0.0f,  0.0f,  1.0f,
		 s, -s,  s,  0.0f,  0.0f,  1.0f,
		 s,  s,  s,  0.0f,  0.0f,  1.0f,
		-s, -s,  s,  0.0f,  0.0f,  1.0f,
		-s,  s,  s,  0.0f,  0.0f,  1.0f,

		// Left face
		-s,  s,  s, -1.0f,  0.0f,  0.0f,
		-s,  s, -s, -1.0f,  0.0f,  0.0f,
		-s, -s, -s, -1.0f,  0.0f,  0.0f,
		-s, -s, -s, -1.0f,  0.0f,  0.0f,
		-s, -s,  s, -1.0f,  0.0f,  0.0f,
		-s,  s,  s, -1.0f,  0.0f,  0.0f,

		// Right face
		 s,  s,  s,  1.0f,  0.0f,  0.0f,
		 s, -s, -s,  1.0f,  0.0f,  0.0f,
		 s,  s, -s,  1.0f,  0.0f,  0.0f,
		 s, -s, -s,  1.0f,  0.0f,  0.0f,
		 s,  s,  s,  1.0f,  0.0f,  0.0f,
		 s, -s,  s,  1.0f,  0.0f,  0.0f,

		// Bottom face
		-s, -s, -s,  0.0f, -1.0f,  0.0f,
		 s, -s, -s,  0.0f, -1.0f,  0.0f,
		 s, -s,  s,  0.0f, -1.0f,  0.0f,
		 s, -s,  s,  0.0f, -1.0f,  0.0f,
		-s, -s,  s,  0.0f, -1.0f,  0.0f,
		-s, -s, -s,  0.0f, -1.0f,  0.0f,

		// Top face
		-s,  s, -s,  0.0f,  1.0f,  0.0f,
		 s,  s,  s,  0.0f,  1.0f,  0.0f,
		 s,  s, -s,  0.0f,  1.0f,  0.0f,
		 s,  s,  s,  0.0f,  1.0f,  0.0f,
		-s,  s, -s,  0.0f,  1.0f,  0.0f,
		-s,  s,  s,  0.0f,  1.0f,  0.0f
	};

	mesh.vertexCount = 36;

	glGenVertexArrays(1, &mesh.VAO);
	glGenBuffers(1, &mesh.VBO);
	glBindVertexArray(mesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	return mesh;
}
