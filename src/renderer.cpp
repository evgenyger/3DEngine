#include "stdafx.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"

Shader test;

GLuint textureId;
GLuint matrixId;

glm::mat4 mvp;

void rInit()
{
	test.loadShaders("shaders/testv.vs", "shaders/testf.fs");
	test.use();

	glEnable(GL_TEXTURE_2D);

	matrixId = glGetUniformLocation(test.id, "MVP");
	textureId = glGetUniformLocation(test.id, "textureSampler");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
}

void rRender()
{
	glDisable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

	mvp = camera->getProjectionMatrix() * glm::mat4_cast(camera->orientation);
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

	glBindTexture(GL_TEXTURE_2D, sky->type->texture.data);
	glBindBuffer(GL_ARRAY_BUFFER, sky->type->vertexBuffer);

	glUniform1i(textureId, 0);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,							// must match the layout in the shader.
		3,							// size
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(Vertex),				// stride
		ATTR_OFFSET(Vertex, pos)	// array buffer offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		ATTR_OFFSET(Vertex, uv)
	);

	glDrawArrays(GL_TRIANGLES, 0, sky->type->vertexCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	for (auto &m : objects)
	{
		mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * m->getModelMatrix();
		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

		glBindTexture(GL_TEXTURE_2D, m->type->texture.data);
		glBindBuffer(GL_ARRAY_BUFFER, m->type->vertexBuffer);

		glUniform1i(textureId, 0);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,							// must match the layout in the shader.
			3,							// size
			GL_FLOAT,					// type
			GL_FALSE,					// normalized?
			sizeof(Vertex),				// stride
			ATTR_OFFSET(Vertex, pos)	// array buffer offset
		);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			ATTR_OFFSET(Vertex, uv)
		);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			ATTR_OFFSET(Vertex, normal)
		);

		glDrawArrays(GL_TRIANGLES, 0, m->type->vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

glm::mat4 const Camera::getProjectionMatrix()
{
	return glm::perspective(fov, (float)w_width / (float)w_height, 0.1f, 1000.0f);
}

glm::mat4 const Camera::getViewMatrix()
{
	return  glm::mat4_cast(orientation) * glm::translate(glm::mat4(1.0f), pos);
}

void Camera::rotate(float angle, const glm::vec3 &axis)
{
	orientation = glm::quat(axis * angle) * orientation;
	glm::normalize(orientation);
}

void Camera::rotate(glm::quat rotation)
{
	orientation = rotation * orientation;
}

void Camera::turn(float angle)
{
	rotate(glm::angleAxis(angle, orientation * glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Camera::pitch(float angle)
{
	rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Camera::yaw(float angle)
{
	rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::roll(float angle)
{
	rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::vec3 Camera::getForward() const
{
	return glm::conjugate(orientation) * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 Camera::getLeft() const
{
	return glm::conjugate(orientation) * glm::vec3(1.0, 0.0f, 0.0f);
}

void Camera::moveForward()
{
	pos += getForward() * speed * deltaTime.asSeconds();
}

void Camera::moveBack()
{
	pos -= getForward() * speed * deltaTime.asSeconds();
}

void Camera::moveLeft()
{
	pos += getLeft() * speed * deltaTime.asSeconds();
}

void Camera::moveRight()
{
	pos -= getLeft() * speed * deltaTime.asSeconds();
}

void Camera::move(glm::vec3 vec)
{
	pos += vec * speed;
}