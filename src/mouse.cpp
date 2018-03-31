#include "stdafx.h"
#include "mouse.h"

void Mouse::handleMove()
{
	pos.x = event.mouseMove.x;
	pos.y = event.mouseMove.y;
}