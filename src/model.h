#pragma once

#include "texture.h"

extern sf::Time deltaTime;

struct Vertex
{
	Vertex() {};

	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Model
{
public:
	Model() {}

	void loadOBJ(std::string path);

	int vertexCount;
	GLuint vertexBuffer;
	Texture texture;
};

class ModelInstance
{
public:
	Model *type;

	glm::vec3 pos;
	glm::quat orientation;

	glm::mat4 getModelMatrix();

	void move(glm::vec3 vec);
	void rotate(float angle, const glm::vec3 &axis);
	void rotate(glm::quat rotation);
	void turn(float angle);
	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);
};