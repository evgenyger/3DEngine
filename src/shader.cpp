#include "stdafx.h"
#include "shader.h"
#include "log.h"

GLuint Shader::loadShaders(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	log(fmt::format("Loading shaders: (v) {} ; (f) {}", vertexPath, fragmentPath));

	/* Load shaders */
	std::string vertexSrc;
	std::string fragmentSrc;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.open(vertexPath);

	if (!vertexFile.is_open())
	{
		error(fmt::format("{} >> loading failed", vertexPath));
		return 0;
	}

	fragmentFile.open(fragmentPath);

	if (!fragmentFile.is_open())
	{
		error(fmt::format("{} >> loading failed", fragmentPath));
		return 0;
	}

	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << vertexFile.rdbuf();
	fShaderStream << fragmentFile.rdbuf();
		
	vertexFile.close();
	fragmentFile.close();

	vertexSrc = vShaderStream.str();
	fragmentSrc = fShaderStream.str();

	const GLchar* vShaderCode = vertexSrc.c_str();
	const GLchar* fShaderCode = fragmentSrc.c_str();
	
	/* Build shaders */
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER), fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GLint success;
	GLchar infoLog[512];

	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		error(fmt::format("{} >> compiling failed:\n {}", vertexPath, infoLog));
		return 0;
	};

	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		error(fmt::format("{} >> compiling failed:\n {}", fragmentPath, infoLog));
		return 0;
	}

	this->id = glCreateProgram();
	glAttachShader(this->id, vertex);
	glAttachShader(this->id, fragment);
	glLinkProgram(this->id);
	
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		error(fmt::format("Linking failed:\n {}", infoLog));
		return 0;
	}

	glDetachShader(this->id, vertex);
	glDetachShader(this->id, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	log(fmt::format("Finished loading shaders"));
}

void Shader::use()
{
	glUseProgram(this->id);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}