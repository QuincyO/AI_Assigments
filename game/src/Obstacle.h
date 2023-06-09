#pragma once
#include "GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle( Vector2 position,std::string key)
		: GameObject( position,key)
	{
	}
	ObjectType GetType() const { return ObjectType::ObstacleType; }

	void Draw();

};