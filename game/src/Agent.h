#pragma once
#include "Rigidbody.h"
#include "Neighborhood.h"
#include "Sprite.h"

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


class Agent : public Sprite
{
public:

    Agent(Vector2 positon, float angularSpeed, float whiskerLength, float maxAccel, float maxSpeed,std::string key) ;

    ~Agent();

    float GetMaxSpeed() const { return m_maxSpeed; }
    float GetMaxAccleration() const { return m_maxAacceleration; }


    Vector2 GetPosition() const { return m_fish->pos; }

    Vector2 GetVelo() const { return m_fish->velo; }
    Vector2 GetAccel() const { return m_fish->accel; }

    int GetWhiskerCount() const { return whiskerCount; }


    bool CheckWhiskerCollision( int index,Vector2 targetPosition);
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
    void Update(Vector2 accleration, float deltaTime);



    void Draw();

    Rigidbody* m_fish;
    Neighborhood Neighborhood;
private:
    float m_maxSpeed; // 350 Px/s 
    float m_maxAacceleration; // 50 Px/s /s
    float slowingRadius = 150.0f;
    float arrivedRadius = 15.0f;
    Texture2D texture;


    float whiskerLength;

    float whiskerAngleL1;
    float whiskerAngleL2;
    float whiskerAngleR1;
    float whiskerAngleR2;

    int whiskerCount = 4;

    bool* detection;
    Vector2* whiskers;



};