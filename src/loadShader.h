#pragma once
#include "ofMain.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

static std::string readFile(const std::string& path) {
	std::string absPath = ofToDataPath(path, true);
	std::ifstream file(absPath);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << absPath << "\n";
		return "";
	}

	std::stringstream ss;
	ss << file.rdbuf();
	std::cout << "Loaded: " << absPath << "\n";
	return ss.str();
}

inline GLuint loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertCode = readFile(vertexPath);
	std::string fragCode = readFile(fragmentPath);

	if (vertCode.empty() || fragCode.empty()) return 0;

	const char* vShaderCode = vertCode.c_str();
	const char* fShaderCode = fragCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cerr << "Vertex shader compilation failed:\n" << infoLog << "\n";
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cerr << "Fragment shader compilation failed:\n" << infoLog << "\n";
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "Shader program linking failed:\n" << infoLog << "\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}
