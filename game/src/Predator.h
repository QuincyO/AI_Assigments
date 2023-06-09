#pragma once
#include "GameObject.h"

class Predator : public GameObject
{
public:
	Predator( Vector2 position)
		: GameObject( position)
	{
	}

	ObjectType GetType() const { return ObjectType::Enemy; }
};

