#pragma once
#include "GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle( Vector2 position)
		: GameObject( position)
	{
	}
	ObjectType GetType() const { return ObjectType::ObstacleType; }

};