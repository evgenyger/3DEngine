#include "stdafx.h"
#include "cvars.h"
#include "shader.h"
#include "renderer.h"
#include "mouse.h"
#include "mapf.h"

sf::Window window;
sf::Clock deltaClock;
sf::Event event;
sf::Time deltaTime;

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

void pExit(int value);
void handleKeyboard();

int main()
{
	std::cout << "<< Starting..." << std::endl;

	initCVars();
	configRead();

	sf::ContextSettings settings;
		settings.depthBits = 24;
		settings.stencilBits = 8;
		settings.minorVersion = 3;
		settings.antialiasingLevel = r_antialiasing;

	window.create(sf::VideoMode(w_width, w_height), "sfml window", (w_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default, settings);

	if ((bool)r_vsync == true)
		window.setVerticalSyncEnabled(true);
	else
		window.setFramerateLimit(r_fpslimit);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "<< [ERROR] GLEW couldn't be initialized!" << std::endl;
		pExit(EXIT_FAILURE);
	}

	initModels();

	rInit();
	
	if (mRead())
		pExit(EXIT_FAILURE);


	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				pExit(EXIT_SUCCESS);

			if (event.type == sf::Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);

				w_width = (int)window.getSize().x;
				w_height = (int)window.getSize().y;
			}

			if (sf::Event::MouseMoved)
				mouse->handleMove();
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		handleKeyboard();

		rRender();
		window.display();

		deltaTime = deltaClock.restart();
	}

	return 0;
}

/* Function called on exit */
void pExit(int value)
{
	window.close();
	configWrite();

	std::cout << "<< Stopping..." << std::endl;

	exit(value);
}

/* Parse keyboard */
void handleKeyboard()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		pExit(EXIT_SUCCESS);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		camera->yaw(-glm::radians(-45.0f) * deltaTime.asSeconds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		camera->yaw(-glm::radians(45.0f) * deltaTime.asSeconds());

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