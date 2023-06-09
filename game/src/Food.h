#pragma once
#include "GameObject.h"

class Food : public GameObject
{
private:
	int foodPoints;
	float eatSpeed = 1.0f/60.0f;
	bool ate;
public:
	Food( Vector2 position,int foodPoints)
		: GameObject( position)
		, foodPoints { foodPoints }
		,ate{false}
	{

	}

	ObjectType GetType() const { return ObjectType::FoodType; }

	bool IsAte() { return ate; }

	void Damage();
};

