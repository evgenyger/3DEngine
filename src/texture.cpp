#include "stdafx.h"
#include "texture.h"
#include "log.h"

Texture::Texture(int x, int y)
{
	data = 0;
	size.x = x;
	size.y = y;
}

Texture::Texture(glm::vec2 size)
{
	this->size = size;
}

void Texture::load(std::string path)
{
	sf::Image img_data;
	if (!img_data.loadFromFile(path))
	{
		warning(fmt::format("{} >> loading failed", path));
		return;
	}

	size.x = img_data.getSize().x;
	size.y = img_data.getSize().y;

	glGenTextures(1, &data);
	glBindTexture(GL_TEXTURE_2D, data);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		size.x, size.y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
}