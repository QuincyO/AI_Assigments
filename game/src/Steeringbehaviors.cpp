#include "Steeringbehaviors.h"
#include "Agent.h"
#include "GameObject.h"



Vector2 Steeringbehaviors::Seek(Agent* agent, Vector2 targetPosition)
{
	Vector2 direction2Target = targetPosition - agent->GetPosition();

	Vector2 desiredVelocity = Normalize(direction2Target) * agent->GetMaxSpeed();

	Vector2 deltaV = desiredVelocity - agent->GetVelo();

	Vector2 desiredAcceleration = Normalize(deltaV) * agent->GetMaxAccleration();

	return deltaV;
}

Vector2 Steeringbehaviors::Flee(Agent* agent, Vector2 targetPosition)
{
	Vector2 direction2Target = targetPosition - agent->GetPosition();

	Vector2 desiredVelocity = Normalize(direction2Target) * agent->GetMaxSpeed();

	Vector2 deltaV = desiredVelocity - agent->GetVelo();

	Vector2 desiredAcceleration = Normalize(deltaV) * agent->GetMaxAccleration();

	return desiredAcceleration * -1;
}

Vector2 Steeringbehaviors::Arrive(Agent* agent, GameObject* object)
{
	Vector2 direction2Target = object->GetPosition() - agent->GetPosition();
	Vector2 desiredVelocity = direction2Target;


	float distance = Distance(object->GetPosition(), agent->m_fish->pos);

	if (distance < agent->Neighborhood.slowingRadius)
	{
		float slowRatio = distance / agent->Neighborhood.slowingRadius;
		if (distance <= agent->Neighborhood.arrivedRadius)
		{
			object->Damage(1.0f / 60.0f);
			if (object->hp < 1)
			{
				object->position = { -500,-500 };
			}
			desiredVelocity = { 0,0 };
		}
		else
		{
			desiredVelocity = Normalize(desiredVelocity) * (agent->GetMaxSpeed() * slowRatio);
		}
		Vector2 steering = desiredVelocity - agent->GetVelo();
		//steering = Normalize(steering) * agent->GetMaxAccleration();
		
		
		return steering;
	}
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
		 agent->m_fish->velo = Rotate(agent->GetVelo(), agent->m_fish->angularSpeed * deltaTime * DEG2RAD);
	}
	else if (detections[2] || detections[3])
	{
		agent->m_fish->velo = Rotate(agent->GetVelo(), -agent->m_fish->angularSpeed * deltaTime * DEG2RAD);
	}

	delete[] detections;
	detections = nullptr;
	return desiredVelocity;
}