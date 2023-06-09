#include "Agent.h"
#include "TextureManager.h"

Agent::Agent(Vector2 positon, float angularSpeed, float whiskerLength, float maxAccel, float maxSpeed)
{
    m_fish = new Rigidbody(angularSpeed, positon);
    m_maxAacceleration = maxAccel;
    m_maxSpeed = maxSpeed;
    this->whiskerLength = whiskerLength;
    whiskers = new Vector2[whiskerCount];
    detection = new bool[whiskerCount];
    whiskerAngleL1 = fmodf(m_fish->rotation - 15 + 360, 360.0f);
    whiskerAngleL2 = fmodf(m_fish->rotation - 30 + 360, 360.0f);
    whiskerAngleR1 = fmodf(m_fish->rotation + 15 + 360, 360.0f);
    whiskerAngleR2 = fmodf(m_fish->rotation + 30 + 360, 360.0f);
    for (int i = 0; i < whiskerCount; i++)
    {
        detection[i] = false;
    }
    //texture = LoadTexture("../game/assets/Fish/fish.png");
}

Agent::~Agent()
{
    delete m_fish;
    m_fish = nullptr;
}

bool Agent::CheckWhiskerCollision(int index,Vector2 targetPosition)
{
    return detection[index] = CheckCollisionLineCircle(m_fish->pos, targetPosition, whiskers[index], 50);
}


void Agent::UpdateWhiskers()
{
    whiskerAngleL1 = fmodf(m_fish->rotation - 15 + 360, 360.0f);
    whiskerAngleL2 = fmodf(m_fish->rotation - 30 + 360, 360.0f);
    whiskerAngleR1 = fmodf(m_fish->rotation + 15 + 360, 360.0f);
    whiskerAngleR2 = fmodf(m_fish->rotation + 30 + 360, 360.0f);


    whiskers[0] = VectorFromAngleDegrees(whiskerAngleL1) * whiskerLength;
    whiskers[1] = VectorFromAngleDegrees(whiskerAngleL2) * whiskerLength;
    whiskers[2] = VectorFromAngleDegrees(whiskerAngleR1) * whiskerLength;
    whiskers[3] = VectorFromAngleDegrees(whiskerAngleR2) * whiskerLength;
}


void Agent::Update(Vector2 accel,float deltaTime)
{
    m_fish->rotation = AngleFromVector(Normalize(m_fish->velo));

    m_fish->accel = Normalize(accel)*m_maxAacceleration;
    UpdateWhiskers();
    float MagOfVelo = Length(m_fish->velo);
    if (MagOfVelo > m_maxSpeed)
    {
        m_fish->velo = m_fish->velo * (m_maxSpeed / MagOfVelo);

    };
    Integrate(deltaTime);
}


void Agent::Update(float deltaTime)
{
    m_fish->rotation = AngleFromVector(Normalize(m_fish->velo));
    UpdateWhiskers();
    float MagOfVelo = Length(m_fish->velo);
    if (MagOfVelo > m_maxSpeed)
    {
        m_fish->velo = m_fish->velo * (m_maxSpeed / MagOfVelo);

    };
    Integrate(deltaTime);
}

void Agent::Draw()
{
    Vector2 veloNorm = Normalize(m_fish->velo);

    DrawTextureEx(TextureManager::GetTexture("fish"), GetPosition(), m_fish->rotation, 1, WHITE);
    //DrawCircleV(m_fish->pos, 50, BLACK);



    //Drawing Neighborhood,Whiskers,Force Vectors
    DrawLineV(m_fish->pos, m_fish->pos + veloNorm * 100,GREEN);
    DrawLineV(m_fish->pos, m_fish->pos + Normalize(m_fish->accel)*100, RED);
    DrawCircleLines(GetPosition().x, GetPosition().y, Neighborhood.slowingRadius, (Neighborhood.inHood) ? GREEN : RED);
    if (Neighborhood.inHood)
    for (int i = 0; i < whiskerCount; i++)
    {
        DrawLineV(m_fish->pos, m_fish->pos + whiskers[i], (detection[i]) ? RED : GREEN);

    }
}