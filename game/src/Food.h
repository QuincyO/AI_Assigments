#pragma once
#include "GameObject.h"

class Food : public GameObject
{
private:
	int foodPoints;
	int eatSpeed;

public:
	Food( Vector2 position, int width, int height)
		: GameObject( position, width, height)
		, foodPoints { 100 }
		, eatSpeed { 5 }
	{

	}

	int getFoodPoints();
	void eatFood(int eatSpeed, float deltaTime);
};

