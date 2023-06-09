#pragma once
#include "raylib.h"

enum ObjectType
{
	Food,
	Obstacle,
	Enemy,

};

class GameObject
{
protected:
	int width;
	int height;

public:
	GameObject(Vector2 position, int hp, int width, int height);
	GameObject(Vector2 position, int width, int height) { this->hp = 0; }
	int hp;
	Vector2 position;

	Vector2 GetPosition() const { return position; }

	virtual ObjectType GetType() const  = 0;

	void Damage(float timeScale) { hp -= 1 * timeScale; }

	virtual void Draw() {}
};

