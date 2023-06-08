#include "Steeringbehaviors.h"
#include "Agent.h"




Vector2 Steeringbehaviors::Seek(Agent* agent, Vector2 targetPosition)
{
	Vector2 direction2Target = targetPosition - agent->GetPosition();

	Vector2 desiredVelocity = Normalize(direction2Target) * agent->GetMaxSpeed();

	Vector2 deltaV = desiredVelocity - agent->GetVelo();

	Vector2 desiredAcceleration = Normalize(deltaV) * agent->GetMaxAccleration();

	return desiredAcceleration;
}

Vector2 Steeringbehaviors::Flee(Agent* agent, Vector2 targetPosition)
{
	Vector2 direction2Target = targetPosition - agent->GetPosition();

	Vector2 desiredVelocity = Normalize(direction2Target) * agent->GetMaxSpeed();

	Vector2 deltaV = desiredVelocity - agent->GetVelo();

	Vector2 desiredAcceleration = Normalize(deltaV) * agent->GetMaxAccleration();

	return desiredAcceleration * -1;
}

Vector2 Steeringbehaviors::Arrive(Agent* agent, Vector2 targetPosition)
{
	Vector2 direction2Target = targetPosition - agent->GetPosition();
	Vector2 desiredVelocity = direction2Target;


	float distance = Distance(targetPosition, agent->m_fish->pos);

	if (distance < agent->Neighborhood.slowingRadius)
	{
		float slowRatio = distance / agent->Neighborhood.slowingRadius;
		if (distance < agent->Neighborhood.arrivedRadius)
		{
			return { 0,0 };
		}
		else
		{
			desiredVelocity = Normalize(desiredVelocity) * (agent->GetMaxSpeed() * slowRatio);
			desiredVelocity = (desiredVelocity - agent->GetVelo()) * agent->GetMaxAccleration();
		}
	}
			return desiredVelocity;
}

//Returns a Velocity Value
Vector2 Steeringbehaviors::Avoid(Agent* agent, Vector2 targetPosition,float deltaTime)
{
	bool* detections = new bool [agent->GetWhiskerCount()];
	Vector2 desiredVelocity = { 0,0 };
	for (int i = 0; i < agent->GetWhiskerCount(); i++)
	{
		detections[i] = agent->CheckWhiskerCollision(i,targetPosition);
	}

	if (detections[0] || detections[1])
	{
		 desiredVelocity = Rotate(agent->GetVelo(), agent->m_fish->angularSpeed * deltaTime * DEG2RAD);
	}

	if (detections[2] || detections[3])
	{
		 desiredVelocity = Rotate(agent->GetVelo(), -agent->m_fish->angularSpeed * deltaTime * DEG2RAD);
	}

	return desiredVelocity;
	delete[] detections;
	detections = nullptr;
}