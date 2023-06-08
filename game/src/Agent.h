#pragma once
#include "Rigidbody.h"
#include "Neighborhood.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

inline Vector2 VectorFromAngleDegrees(float angleDegrees)
{
    return Vector2{ cosf(angleDegrees * DEG2RAD),sinf(angleDegrees * DEG2RAD) };
}

inline float AngleFromVector(Vector2 direction)
{
    float angle = atan2f(direction.y, direction.x) * RAD2DEG;
    angle = fmodf(angle + 360, 360.0f);
    return angle;
}


inline bool CheckCollisionLineCircle(Vector2 agentPosition, Vector2 obstaclePosition, Vector2 lineToCheckAgainst, float circleRadius)
{
    Vector2 NearestPointOnVector = NearestPoint(agentPosition, agentPosition + lineToCheckAgainst, obstaclePosition);

    float distanceFromObstacleToPoint = Distance(NearestPointOnVector, obstaclePosition);
    return  (distanceFromObstacleToPoint <= circleRadius);
}

inline Vector2 WrapAroundScreen(Vector2 position)
{
    Vector2 outPosition =
    {
        fmodf(position.x + SCREEN_WIDTH,SCREEN_WIDTH),
        fmodf(position.y + SCREEN_HEIGHT,SCREEN_HEIGHT)
    };
    return outPosition;
}


class Agent
{
public:

    Agent(Vector2 positon, float angularSpeed, float whiskerLength, float maxAccel, float maxSpeed);

    ~Agent();

    float GetMaxSpeed() const { return m_maxSpeed; }
    float GetMaxAccleration() const { return m_maxAacceleration; }


    Vector2 GetPosition() const { return m_fish->pos; }

    Vector2 GetVelo() const { return m_fish->velo; }
    Vector2 GetAccel() const { return m_fish->accel; }

    int GetWhiskerCount() const { return whiskerCount; }


    bool CheckWhiskerCollision( int index,Vector2 targetPosition);

    void Avoid(Vector2 obstacle, float deltaTime, float radiusOfObstacle)
    {
        //direction = Rotate(direction, 50 * dt * DEG2RAD);

        for (int i = 0; i < whiskerCount; i++)
        {
            detection[i] = CheckCollisionLineCircle(m_fish->pos, obstacle, whiskers[i], radiusOfObstacle);
        }

        if (detection[1] || detection[0])
        {
            m_fish->velo = Rotate(m_fish->velo, m_fish->angularSpeed * deltaTime * DEG2RAD);
        }

        if (detection[2] || detection[3])
        {
            m_fish->velo = Rotate(m_fish->velo, -m_fish->angularSpeed * deltaTime * DEG2RAD);
        }

    };

    void UpdateWhiskers();
    void Integrate(float deltaTime)
    {
        m_fish->pos = m_fish->pos + (m_fish->velo * deltaTime) + ((m_fish->accel * 0.5f) * deltaTime * deltaTime);
        m_fish->velo = m_fish->velo + m_fish->accel * deltaTime;
        m_fish->pos = WrapAroundScreen(m_fish->pos);
        m_fish->accel = {};
    }

    //Updates Movement
    void Update(float deltaTime);

    Vector2 Seek(Vector2 targetPosition,float dt)
    {
        Vector2 direction2Target = targetPosition - m_fish->pos;

        Vector2 desiredVelocity = Normalize(direction2Target) * GetMaxSpeed();

        Vector2 deltaV = desiredVelocity - GetVelo();

        Vector2 desiredAcceleration = Normalize(deltaV) * GetMaxAccleration();
        m_fish->accel = desiredAcceleration;
        return desiredAcceleration;
    }

    Vector2 Flee(float deltaTime, Vector2 targetPosition)
    {
        Vector2 deltaAccel = (Normalize(targetPosition - m_fish->pos) * m_maxSpeed  - m_fish->velo) * -1;
        return deltaAccel;
    }



    void Draw();

    Rigidbody* m_fish;
    Neighborhood Neighborhood;
private:
    float m_maxSpeed; // 350 Px/s 
    float m_maxAacceleration; // 50 Px/s /s
    float slowingRadius = 150.0f;
    float arrivedRadius = 15.0f;

    float whiskerLength;

    float whiskerAngleL1;
    float whiskerAngleL2;
    float whiskerAngleR1;
    float whiskerAngleR2;

    int whiskerCount = 4;

    bool* detection;
    Vector2* whiskers;



};