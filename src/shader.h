#pragma once

#include "stdafx.h"

GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path);

class Shader
{
public:
	GLuint id;
	GLuint loadShaders(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();

	void Shader::setBool(const std::string &name, bool value) const;
	void Shader::setInt(const std::string &name, int value) const;
	void Shader::setFloat(const std::string &name, float value) const;
	void Shader::setVec2(const std::string &name, const glm::vec2 &value) const;
	void Shader::setVec3(const std::string &name, const glm::vec3 &value) const;
	void Shader::setVec4(const std::string &name, const glm::vec4 &value) const;
	void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const;
	void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const;
	void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const;
};