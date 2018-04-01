#include "stdafx.h"
#include "mouse.h"
#include "renderer.h"

void Mouse::handleMove()
{
	ppos = pos;
	pos.x = event.mouseMove.x;
	pos.y = event.mouseMove.y;
}