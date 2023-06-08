#pragma once
#include "GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle(Texture2D texture, Vector2 position, int width, int height)
		: GameObject(texture, position, width, height)
	{

	}
};