#pragma once
#include "GameObject.h"

class Predator : public GameObject
{
public:
	Predator( Vector2 position, int width, int height)
		: GameObject( position, width, height)
	{

	}
};

