#pragma once

class Texture
{
public:
	Texture();
	Texture(int x, int y);
	Texture(glm::vec2 size);
	void load(std::string path);

	GLuint data;
	glm::vec2 size;
};
