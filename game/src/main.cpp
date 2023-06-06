#include "rlImGui.h"
#include "Math.h"
#include <iostream>
#include <vector>
#include "sprite.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//        position = position + (m_fish->GetVelocity() * deltaTime) + ((acceleration * 0.5f) * deltaTime * deltaTime);
//        position = WrapAroundScreen(position);
//        m_fish->SetPosition(position);
//        m_fish->SetVelocity(m_fish->GetVelocity() + acceleration * deltaTime);

bool CheckCollisionLineCircle(Vector2 agentPosition, Vector2 obstaclePosition, Vector2 lineToCheckAgainst, float circleRadius)
{
    Vector2 NearestPointOnVector = NearestPoint(agentPosition, agentPosition + lineToCheckAgainst, obstaclePosition);

    float distanceFromObstacleToPoint = Distance(NearestPointOnVector, obstaclePosition);
    return  (distanceFromObstacleToPoint <= circleRadius);
}

Vector2 WrapAroundScreen(Vector2 position)
{
    Vector2 outPosition =
    {
        fmodf(position.x + SCREEN_WIDTH,SCREEN_WIDTH),
        fmodf(position.y + SCREEN_HEIGHT,SCREEN_HEIGHT)
    };
    return outPosition;
}
float AngleFromVector(Vector2 direction)
{
    float angle = atan2f(direction.y, direction.x) * RAD2DEG;
    angle = fmodf(angle + 360, 360.0f);
    return angle;
}

Vector2 VectorFromAngleDegrees(float angleDegrees)
{
    return Vector2{ cosf(angleDegrees * DEG2RAD),sinf(angleDegrees * DEG2RAD) };
}

struct RigidBody
{
    Vector2 pos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };
    Vector2 accel = {};
    Vector2 velo = {};
    Vector2 dir = {};
    float rotation;
    float angularSpeed;
};



class Agent : public Sprite
{
public:


    Agent(float r1, float maxAccel, float maxSpeed) :Sprite()
    {
        m_fish = new RigidBody();
        m_maxAacceleration = maxAccel;
        m_maxSpeed = maxSpeed;
        whiskerLengthL1 = r1;
        whiskerLengthL2 = r1;
        whiskerLengthR1 = r1;
        whiskerLengthR2 = r1;
        m_fish->angularSpeed = 100;
        whiskerAngleL1 = fmodf(m_fish->rotation - 15 + 360, 360.0f);
        whiskerAngleL2 = fmodf(m_fish->rotation - 30 + 360, 360.0f);
        whiskerAngleR1 = fmodf(m_fish->rotation + 15 + 360, 360.0f);
        whiskerAngleR2 = fmodf(m_fish->rotation + 30 + 360, 360.0f);


        whiskers = new Vector2[whiskerCount];
        detection = new bool[whiskerCount];


       

    }

    void StartUp(const char* filepath)
    {

        Sprite::LoadImage(filepath);
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.width = texture.width;
        srcRect.height = texture.height;

        dstRect;
        dstRect;
        dstRect.width = srcRect.width;
        dstRect.height = srcRect.height;
    }

    ~Agent()
    {
        delete m_fish;
        m_fish = nullptr;
    }





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

    }

    void UpdateWhiskers()
    {
        whiskerAngleL1 = fmodf(m_fish->rotation - 15 + 360, 360.0f);
        whiskerAngleL2 = fmodf(m_fish->rotation - 30 + 360, 360.0f);
        whiskerAngleR1 = fmodf(m_fish->rotation + 15 + 360, 360.0f);
        whiskerAngleR2 = fmodf(m_fish->rotation + 30 + 360, 360.0f);


        whiskers[0] = VectorFromAngleDegrees(whiskerAngleL1) * whiskerLengthL1;
        whiskers[1] = VectorFromAngleDegrees(whiskerAngleL2) * whiskerLengthL2;
        whiskers[2] = VectorFromAngleDegrees(whiskerAngleR1) * whiskerLengthR1;
        whiskers[3] = VectorFromAngleDegrees(whiskerAngleR2) * whiskerLengthR2;




    }

    //Updates Movement
    void Update(float deltaTime)
    {
        m_fish->rotation = AngleFromVector(Normalize(m_fish->velo));
        UpdateWhiskers();







        float MagOfVelo = Length(m_fish->velo);
        if (MagOfVelo > m_maxSpeed)
        {
            m_fish->velo = m_fish->velo * (m_maxSpeed / MagOfVelo);
        };
        m_fish->pos = m_fish->pos + (m_fish->velo * deltaTime) + ((m_fish->accel * 0.5f) * deltaTime * deltaTime);
        m_fish->velo = m_fish->velo + m_fish->accel * deltaTime;
        m_fish->pos = WrapAroundScreen(m_fish->pos);
        m_fish->accel = {};

        //  m_fish->dir = RotateTowards(m_fish->dir, Normalize(m_fish->velo), m_fish->angularSpeed * deltaTime);
    }


    //Returns Acceleration Vector Towards obstacle
    Vector2 Seek(Vector2 const  targetPosition, float deltaTIme)
    {
        Vector2 deltaAccel = Normalize(targetPosition - m_fish->pos) * m_maxSpeed - m_fish->velo;

        m_fish->rotation = AngleFromVector(targetPosition - m_fish->pos);
        //  m_fish->dir = VectorFromAngleDegrees(m_fish->rotation);
        m_fish->accel = deltaAccel;
        return deltaAccel;
    }


    Vector2 Flee(float deltaTime, Vector2 targetPosition)
    {
        Vector2 deltaAccel = (Normalize(targetPosition - m_fish->pos) * m_maxSpeed - m_fish->velo) * -1;
        return deltaAccel;
    }

    void Draw()
    {
        Vector2 veloNorm = Normalize(m_fish->velo);
        dstRect.x = m_fish->pos.x;
        dstRect.y = m_fish->pos.y;
        //DrawCircleV(m_fish->pos, circleRadius, BLACK);

        DrawTexturePro(texture, srcRect, dstRect,{(float)texture.width/2,(float)texture.height/2}, m_fish->rotation, WHITE);

        DrawLineV(m_fish->pos, m_fish->pos + veloNorm * 100, RED);
        for (int i = 0; i < whiskerCount; i++)
        {
            DrawLineV(m_fish->pos, m_fish->pos + whiskers[i], (detection[i]) ? RED : GREEN);

        }
    }

private:
    float m_maxSpeed; // 350 Px/s 
    float m_maxAacceleration; // 50 Px/s /s
    float whiskerLengthL1;
    float whiskerLengthL2;
    float whiskerLengthR1;
    float whiskerLengthR2;
    RigidBody* m_fish;
    float circleRadius = 40;


    float whiskerAngleL1;
    float whiskerAngleL2;
    float whiskerAngleR1;
    float whiskerAngleR2;

    int whiskerCount = 4;

    bool* detection;
    Vector2* whiskers;
};







int main(void)
{
    std::vector<Agent*> agents;
    Agent* fish1 = new Agent(100, 125, 200);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);


    float timer = 0;

    const float radius = 30;
    const float whiskerLength = 300;

    fish1->StartUp("../game/assets/Fish/fish.png");


    Vector2 position = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };//in px
    Vector2 velocity = { 0,0 }; //In px/s
    float maxSpeed = 10;
    float maxAccel = 150;
    Vector2 acceleration = { 0,0 }; //In px/s/s
    Vector2 direction = { 13.0f,25.0f };


    Vector2 mousePOS = { 0,0 };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);
        rlImGuiBegin();

        const float dt = GetFrameTime();
        mousePOS = GetMousePosition();





        //  ImGui::SliderFloat2("position", &(position.x), 0, SCREEN_WIDTH);
        //  ImGui::SliderFloat2("velocity", &(velocity.x), -maxSpeed, maxSpeed);
        //  ImGui::SliderFloat2("Acceleration", &(acceleration.x), -maxAccel, maxAccel);
        //  ImGui::SliderFloat("Max Acceleration", &maxAccel, 1, 1500); 
        //  ImGui::SliderFloat("Max Speed", &maxSpeed, -1, 1500);



        fish1->Seek(mousePOS, dt);

        fish1->Avoid(position, dt, radius);
        fish1->Update(dt);
        fish1->Draw();






        position = WrapAroundScreen(position);
        DrawCircleV(mousePOS, radius, RED);
        DrawCircleV(position, 25, BLUE);


        //    DrawLineV(position, position + Vector2{100,0}, BLACK);
        //    DrawLineV(position, position + direction *100, PINK);
        //    DrawLineV(position, position + wiskerLeft, (CheckCollisionLineCircle(position,mousePOS,wiskerLeft,radius)) ? RED : GREEN);
        //    DrawLineV(position, position + wiskerRight, GREEN);
        //    //DrawCircleV(nearestPointC, 5, BLACK);



        //  DrawLineV(position, position + acceleration, RED);
        //  DrawLineV(position, position + desiredVelocity, PURPLE);



        timer += dt;
        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    return 0;

}