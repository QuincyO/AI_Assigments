#include "rlImGui.h"
#include "Math.h"
#include <iostream>
#include <vector>
#include "TextureManager.h"
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

class RigidBody
{
public:

    Vector2 pos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };
    Vector2 accel = {};
    Vector2 velo = {};
    Vector2 dir = {};
    float rotation;
    float angularSpeed;
    float m_slowingRatio = 1.0f;

    RigidBody() = default;

    RigidBody(const Vector2& pos, const Vector2& accel, const Vector2& velo, const Vector2& dir, float rotation, float angularSpeed)
        : pos(pos), accel(accel), velo(velo), dir(dir), rotation(rotation), angularSpeed(angularSpeed)
    {
    }
};



class Agent
{
public:

    Agent(float r1, float r2, float l1, float l2, float maxAccel, float maxSpeed)
    {
        m_fish = new RigidBody();
        m_maxAacceleration = maxAccel;
        m_maxSpeed = maxSpeed;
        whiskerLengthL1 = l1;
        whiskerLengthL2 = l2;
        whiskerLengthR1 = r1;
        whiskerLengthR2 = r2;
        m_fish->angularSpeed = 100;

        whiskers = new Vector2[whiskerCount];
        detection = new bool[whiskerCount];

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
        Vector2 deltaAccel = Normalize(targetPosition - m_fish->pos) * m_maxSpeed * m_slowingRatio - m_fish->velo;

        m_fish->rotation = AngleFromVector(targetPosition - m_fish->pos);
        //  m_fish->dir = VectorFromAngleDegrees(m_fish->rotation);
        m_fish->accel = deltaAccel;
        return deltaAccel;
    }


    Vector2 Flee(float deltaTime, Vector2 targetPosition)
    {
        Vector2 deltaAccel = (Normalize(targetPosition - m_fish->pos) * m_maxSpeed * m_slowingRatio - m_fish->velo) * -1;
        return deltaAccel;
    }

    void Arrive(Vector2 const  targetPosition)
    {
        if (Length(targetPosition - m_fish->pos) < 480)
            m_slowingRatio = (Length(targetPosition - m_fish->pos) / 480);
        else if(Length(targetPosition - m_fish->pos) < 60)
            m_fish->velo = Normalize(targetPosition - m_fish->pos) * (Length(targetPosition - m_fish->pos) / 60);
        else if(Length(targetPosition - m_fish->pos) < 30)
            m_fish->velo = Normalize(targetPosition - m_fish->pos);

    }

    void Draw()
    {
        Vector2 veloNorm = Normalize(m_fish->velo);

        //DrawCircleV(m_fish->pos, circleRadius, BLACK);
        DrawLineV(m_fish->pos, m_fish->pos + veloNorm * 100, RED);
        for (int i = 0; i < whiskerCount; i++)
        {
            DrawLineV(m_fish->pos, m_fish->pos + whiskers[i], (detection[i]) ? RED : GREEN);

        }
    }

public:
    float m_maxSpeed; // 350 Px/s 
    float m_maxAacceleration; // 50 Px/s /s
    float m_slowingRatio;
    float whiskerLengthL1;
    float whiskerLengthL2;
    float whiskerLengthR1;
    float whiskerLengthR2;
    RigidBody* m_fish;

    float circleRadius = 40.0f;
    float arrivingZoneCircleRadius = 120.0f;


    float whiskerAngleL1;
    float whiskerAngleL2;
    float whiskerAngleR1;
    float whiskerAngleR2;

    int whiskerCount = 4;

    bool* detection;
    Vector2* whiskers;

    Vector2 whiskerLeft1;
    Vector2 whiskerLeft2;
    Vector2 whiskerRight1;
    Vector2 whiskerRight2;
};




int main(void)
{
    std::vector<Agent*> agents;
    Agent* fish1 = new Agent(100, 100, 100, 100, 125, 200); 
    RigidBody* food1 = new RigidBody({ 100.0f, 100.0f }, {0,0}, {0,0}, {0,0}, 0.0f, 0.0f);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);

    TextureManager gameTextures;

    gameTextures.Load("../game/assets/Fish/fish.png", "fishTexture");
    gameTextures.Load("../game/assets/Fish/obstacle.png", "obstacleTexture");

    float timer = 0;

    const float radius = 30;
    const float whiskerLength = 300;


    Vector2 position = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };//in px
    Vector2 velocity = { 0,0 }; //In px/s
    float maxSpeed = 10;
    float maxAccel = 150;
    Vector2 acceleration = { 0,0 }; //In px/s/s
    Vector2 direction = { 13.0f,25.0f };


    Vector2 mousePOS = { 0,0 };

    Rectangle fishsrc = { 0,0,64,64 };


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);
        rlImGuiBegin();

        const float dt = GetFrameTime();
        mousePOS = GetMousePosition();


        Rectangle fishdst = { fish1->m_fish->pos.x, fish1->m_fish->pos.y, 64, 64 };
        Vector2 fishOrigin = { fishdst.width / 2, fishdst.height / 2 };

        float angle = AngleFromVector(direction);

        float wiskerAngleLeft = fmodf(angle - 30 + 360, 360.0f);
        float wiskerAngleRight = fmodf(angle + 30 + 360, 360.0f);

        Vector2 wiskerLeft = VectorFromAngleDegrees(wiskerAngleLeft) * whiskerLength;
        Vector2 wiskerRight = VectorFromAngleDegrees(wiskerAngleRight) * whiskerLength;

        bool leftCollision = CheckCollisionLineCircle(position, mousePOS, wiskerLeft, radius);
        bool rightCollision = CheckCollisionLineCircle(position, mousePOS, wiskerRight, radius);

        if (leftCollision)
        {
            direction = Rotate(direction, 50 * dt * DEG2RAD);
        }

        if (rightCollision)
        {
            direction = Rotate(direction, -50 * dt * DEG2RAD);
        }



        //  ImGui::SliderFloat2("position", &(position.x), 0, SCREEN_WIDTH);
        //  ImGui::SliderFloat2("velocity", &(velocity.x), -maxSpeed, maxSpeed);
        //  ImGui::SliderFloat2("Acceleration", &(acceleration.x), -maxAccel, maxAccel);
        //  ImGui::SliderFloat("Max Acceleration", &maxAccel, 1, 1500); 
        //  ImGui::SliderFloat("Max Speed", &maxSpeed, -1, 1500);


        fish1->Seek(mousePOS, dt);

        fish1->Avoid(position, dt, radius);
        fish1->Arrive(mousePOS);

        fish1->Update(dt);
        if (Length(mousePOS - fish1->m_fish->velo) < 30)
            fish1->m_fish->velo = { 0,0 };
        fish1->Draw();
       





        position = WrapAroundScreen(position);
        DrawCircleV(mousePOS, radius, RED);
        DrawCircleV(position, 25, BLUE);
        DrawCircleV(food1->pos, 40, YELLOW);



        DrawTexturePro(gameTextures.textures["fishTexture"], fishsrc, fishdst, fishOrigin, 1, RED);
        DrawTexturePro(gameTextures.textures["obstacleTexture"], { 0,0,64,64 }, { food1->pos.x, food1->pos.y, 64, 64 }, { food1->pos.x, food1->pos.y }, 1, RED);

        //    DrawLineV(position, position + Vector2{100,0}, BLACK);
        //    DrawLineV(position, position + direction *100, PINK);
        //    DrawLineV(position, position + wiskerLeft, (CheckCollisionLineCircle(position,mousePOS,wiskerLeft,radius)) ? RED : GREEN);
        //    DrawLineV(position, position + wiskerRight, GREEN);
        //    //DrawCircleV(nearestPointC, 5, BLACK);

        DrawText(TextFormat("Angle: %f.1", angle), 200, position.y + 45, 20, RED);
        DrawText(TextFormat("Wisker Angle Green : %f.1", wiskerAngleLeft), 200, position.y + 65, 20, RED);
        DrawText(TextFormat("Wisker Angle Blue : %f.1", wiskerAngleRight), 200, position.y + 85, 20, RED);

        //  DrawLineV(position, position + acceleration, RED);
        //  DrawLineV(position, position + desiredVelocity, PURPLE);



        timer += dt;
        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    return 0;

}