#include "stdafx.h"
#include "cvars.h"
#include "shader.h"
#include "renderer.h"
#include "mouse.h"
#include "mapf.h"
#include "log.h"

sf::Window	window;
sf::Clock	dclock;
sf::Event	event;
sf::Time	deltaTime, 
			t1 = sf::Time::Zero, t2;

Mouse *mouse = new Mouse;
Camera *camera = new Camera;

GLuint programId;

extern IntCVar w_width;
extern IntCVar w_height;
extern IntCVar w_fullscreen;

extern IntCVar r_fpslimit;
extern IntCVar r_vsync;
extern IntCVar r_antialiasing;

extern ModelInstance *cannon;
extern ModelInstance *target;

void pInit();
void pExit(int value);
void _terminate();
void handleKeyboard();

int main()
{
	pInit();

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				pExit(EXIT_SUCCESS);

			if (event.type == sf::Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);
				rUpdateFBO();

				w_width = (int)window.getSize().x;
				w_height = (int)window.getSize().y;
			}

			if (sf::Event::MouseMoved)
			{
				mouse->handleMove();
				int dx = mouse->pos.x - mouse->ppos.x;
				int dy = mouse->pos.y - mouse->ppos.y;
			}
		}

		handleKeyboard();

		rRender();
		window.display();

		t2 = dclock.getElapsedTime();
		deltaTime = t2 - t1;
		t1 = t2;
	}

	return 0;
}

/* Init function */
void pInit()
{
	std::set_terminate(_terminate);

	logInit();

	log("Starting...");

	initCVars();
	configRead();

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.minorVersion = 3;
	settings.antialiasingLevel = r_antialiasing;

	window.create(sf::VideoMode(w_width, w_height), "random shit", (w_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default, settings);

	if ((bool)r_vsync == true)
		window.setVerticalSyncEnabled(true);
	else
		window.setFramerateLimit(r_fpslimit);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		error("GLEW was not initialized");
		pExit(EXIT_FAILURE);
	}

	initModels();

	rInit();

	if (mRead())
		pExit(EXIT_FAILURE);
}

/* Function called on exit */
void pExit(int value)
{
	window.close();
	configWrite();

	log("Stopping...");

	exit(value);
}

/* Terminate function */
void _terminate()
{
	log("PROGRAM TERMINATED");
	pExit(EXIT_FAILURE);
}

/* Parse keyboard */
void handleKeyboard()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		pExit(EXIT_SUCCESS);

	/* kinda useless
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		camera->yaw(-glm::radians(-45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		camera->yaw(-glm::radians(45.0f) * deltaTime.asSeconds());
	*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
		camera->move(glm::vec3(0, -1, 0) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
		camera->move(glm::vec3(0, 1, 0) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		camera->turn(-glm::radians(45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		camera->turn(glm::radians(45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		camera->pitch(-glm::radians(45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		camera->pitch(glm::radians(45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		camera->roll(-glm::radians(45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		camera->roll(glm::radians(45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		camera->moveForward();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		camera->moveBack();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		camera->moveLeft();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		camera->moveRight();
}