#include "stdafx.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "log.h"

Shader sh_main, sh_screen;

GLuint fbo;

glm::mat4 mvp;

void rUpdateFBO();
void rRenderSky();

GLuint g_quadVAO, g_quadVBO;
static const GLfloat g_quad[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

void rInit()
{
	sh_main.loadShaders("shaders/testv.vs", "shaders/testf.fs");
	sh_screen.loadShaders("shaders/screen.vs", "shaders/screen.fs");

	glEnable(GL_TEXTURE_2D);

	sh_main.setInt("MVP", 0);
	sh_main.setInt("textureSampler", 0);
	sh_screen.setInt("screenTexture", 0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &fbo);

	rUpdateFBO();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		error("Framebuffer is not complete");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &g_quadVAO);
	glBindVertexArray(g_quadVAO);

	glGenBuffers(1, &g_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad), g_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void rUpdateFBO()
{
	glBindTexture(GL_TEXTURE_2D, fbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.getSize().x, window.getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void rRender()
{
	sh_main.use();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	rRenderSky();

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	for (auto &m : objects)
	{
		mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * m->getModelMatrix();
		sh_main.setMat4("MVP", mvp);

		glBindTexture(GL_TEXTURE_2D, m->type->texture.data);
		glBindBuffer(GL_ARRAY_BUFFER, m->type->vertexBuffer);

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
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ATTR_OFFSET(Vertex, uv));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3,	GL_FLOAT, GL_FALSE,	sizeof(Vertex),	ATTR_OFFSET(Vertex, normal));

		glDrawArrays(GL_TRIANGLES, 0, m->type->vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	/* Render framebuffer */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	sh_screen.use();

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(g_quadVAO);
	glBindTexture(GL_TEXTURE_2D, fbo);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void rRenderSky()
{
	glDisable(GL_STENCIL_TEST);
	glFrontFace(GL_CW);

	mvp = camera->getProjectionMatrix() * glm::mat4_cast(camera->orientation);
	sh_main.setMat4("MVP", mvp);

	glBindTexture(GL_TEXTURE_2D, sky->type->texture.data);
	glBindBuffer(GL_ARRAY_BUFFER, sky->type->vertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3,	GL_FLOAT, GL_FALSE, sizeof(Vertex), ATTR_OFFSET(Vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ATTR_OFFSET(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, sky->type->vertexCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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