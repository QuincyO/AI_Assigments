#pragma once
#include "raylib.h"
#include <iostream>
#include "Sprite.h"

enum ObjectType
{
	FoodType,
	ObstacleType,
	Enemy,

};

class GameObject : public Sprite
{
protected:
	Vector2 position;

public:
	GameObject(Vector2 position,std::string key)
		:Sprite(key)
		,position{position}
	{}

	Vector2 GetPosition() const { return position; }

	void RemoveFromScreen() { position = { -500,-500 }; }

	virtual ObjectType GetType() const  = 0;

	virtual bool IsAte() { return false; };

	virtual void Damage() {};

	virtual void Draw() = 0;
};

