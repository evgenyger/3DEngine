#pragma once

extern sf::Event event;

class Mouse
{
public:
	Mouse() :	pos(0), ppos(0),
				speed(0.05f)
	{}

	glm::vec2 pos, ppos;
	float speed;

	void handleMove();
};