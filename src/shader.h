#pragma once

#include "stdafx.h"

GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path);

class Shader
{
public:
	GLuint id;
	GLuint loadShaders(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();
};