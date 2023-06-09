#pragma once
#include "GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle( Vector2 position, int width, int height)
		: GameObject( position, width, height)
	{

	}
};