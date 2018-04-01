#include "stdafx.h"
#include "log.h"

GLuint loadShaders(const char *vertex_file_path, const char *fragment_file_path) 
{
	/* Create the shaders */
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	/* Read the Vertex Shader code from the file */
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);

	if (!VertexShaderStream.is_open())
	{
		error(fmt::format("{} >> Failed to load vertex shader!", vertex_file_path));
		getchar();
		return 0;
	}

	std::string Line = "";
	while (getline(VertexShaderStream, Line))
		VertexShaderCode += "\n" + Line;

	VertexShaderStream.close();

	/* Read the Fragment Shader code from the file */
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);

	if (!FragmentShaderStream.is_open()) 
	{
		error(fmt::format("{} >> Failed to load fragment shader!", fragment_file_path));
		getchar();
		return 0;
	}

	Line = "";
	while (getline(FragmentShaderStream, Line))
		FragmentShaderCode += "\n" + Line;
	FragmentShaderStream.close();

	GLint Result = GL_FALSE;
	int InfoLogLength;


	/* Compile Vertex Shader */
	log(fmt::format("{} >> Compiling vertex shader!", vertex_file_path));

	char const *VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	/* Check Vertex Shader */
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) 
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		error(fmt::format("{} >> Vertex shader error: {}", vertex_file_path, &VertexShaderErrorMessage[0]));
	}

	/* Compile Fragment Shader */
	log(fmt::format("{} >> Compiling fragment shader!", fragment_file_path));

	char const *FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	/* Check Fragment Shader */
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		error(fmt::format("{} >> Fragment shader error: {}", fragment_file_path, &FragmentShaderErrorMessage[0]));
	}


	/* Link the program */
	log("Linking program...");

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	/* Check the program */
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) 
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		error(fmt::format("Shader error: {}", &ProgramErrorMessage[0]));
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	log(fmt::format("Finished loading shaders! ProgramID: {}", ProgramID));

	return ProgramID;
}