#pragma once
#include "rlImGui.h"

class Agent;

class Steeringbehaviors
{
public:

	static Vector2 Seek(Agent* agent, Vector2 targetPosition);
	static Vector2 Flee(Agent* agent, Vector2 targetPosition);
	static Vector2 Arrive(Agent* agent, Vector2 targetPosition);
	static Vector2 Avoid(Agent* agent, Vector2 targetPosition,float deltaTime);
};

