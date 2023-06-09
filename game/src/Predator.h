#pragma once
#include "GameObject.h"

class Predator : public GameObject
{
public:
	Predator( Vector2 position,std::string key)
		: GameObject( position,key)
	{
	}

	ObjectType GetType() const { return ObjectType::Enemy; }
	void Draw() override;
};

