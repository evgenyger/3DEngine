#pragma once

#include "cvars.h"
#include "mouse.h"
#include "mapf.h"

class Camera;
class ModelInstance;

extern IntCVar w_width;
extern IntCVar w_height;
extern GLuint programId;

extern Camera *camera;
extern Mouse *mouse;
extern sf::Time deltaTime;

extern ModelInstance *sky;

extern std::vector<std::unique_ptr<ModelInstance>> objects;

#define ATTR_OFFSET(T, a) ((void*)offsetof(T, a))

void rInit();
void rRender();

class Camera
{
public:
	Camera() : fov(45.0f),
		speed(50.0f)
	{}

	glm::vec3 pos;
	glm::quat orientation;

	float fov;
	float speed;

	glm::mat4 const getProjectionMatrix();
	glm::mat4 const getViewMatrix();

	void rotate(float angle, const glm::vec3 &axis);
	void rotate(glm::quat rotation);

	void turn(float angle);
	void yaw(float angle);
	void pitch(float angle);
	void roll(float angle);

	glm::vec3 getForward() const;
	glm::vec3 getLeft() const;

	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	void move(glm::vec3 vec);
};