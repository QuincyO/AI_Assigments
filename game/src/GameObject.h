#pragma once
#include "raylib.h"

class GameObject
{
private:
	Texture2D texture;
	Vector2 position;
	int width;
	int height;

public:
	GameObject(Texture2D texture, Vector2 position, int width, int height);
};

