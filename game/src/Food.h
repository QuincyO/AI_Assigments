#pragma once
#include "GameObject.h"

class Food : public GameObject
{
private:
	float foodPoints;
	float eatSpeed = 1.0f/60.0f;
	bool ate;
public:
	Food( Vector2 position,float foodPoints,std::string key)
		: GameObject( position,key)
		, foodPoints { foodPoints }
		,ate{false}
	{

	}

	ObjectType GetType() const { return ObjectType::FoodType; }

	bool IsAte() { return ate; }

	void Damage();

	void Draw();
};

