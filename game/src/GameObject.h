#pragma once
#include "raylib.h"
#include <iostream>

enum ObjectType
{
	FoodType,
	ObstacleType,
	Enemy,

};

class GameObject
{
protected:
	Vector2 position;


public:
	GameObject(Vector2 position)
		:position{position}
	{}

	Vector2 GetPosition() const { return position; }

	void RemoveFromScreen() { position = { -500,-500 }; }

	virtual ObjectType GetType() const  = 0;

	virtual bool IsAte() { std::cout << "Parent" << std::endl; return NULL; };
	virtual void Damage() {};

	virtual void Draw() {}
};

