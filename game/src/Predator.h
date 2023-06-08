#pragma once
#include "GameObject.h"

class Predator : public GameObject
{
public:
	Predator(Texture2D texture, Vector2 position, int width, int height)
		: GameObject(texture, position, width, height)
	{

	}
};

