#include "rlImGui.h"
#include "Math.h"
#include <iostream>
#include <vector>
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

    void Integrate(float deltaTime)
    {
        m_fish->pos = m_fish->pos + (m_fish->velo * deltaTime) + ((m_fish->accel * 0.5f) * deltaTime * deltaTime);
        m_fish->velo = m_fish->velo + m_fish->accel * deltaTime;
        m_fish->pos = WrapAroundScreen(m_fish->pos);
        m_fish->accel = {};
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


        

        //  m_fish->dir = RotateTowards(m_fish->dir, Normalize(m_fish->velo), m_fish->angularSpeed * deltaTime);
        Integrate(deltaTime);
    }


    //Returns Acceleration Vector Towards obstacle
    Vector2 Seek(Vector2 const  targetPosition, float deltaTime)
    {
        Vector2 desiredVelocity = Normalize(targetPosition - m_fish->pos) * m_maxSpeed;
        Vector2 deltaV = desiredVelocity - m_fish->velo;

        Vector2 accel = Normalize(deltaV) * m_maxAacceleration;

        //m_fish->rotation = AngleFromVector(targetPosition - m_fish->pos);
        m_fish->accel = accel;
       // Integrate(deltaTime, accel);
        return accel;
    }


    Vector2 Flee(float deltaTime, Vector2 targetPosition)
    {
        Vector2 deltaAccel = (Normalize(targetPosition - m_fish->pos) * m_maxSpeed * m_slowingRatio - m_fish->velo) * -1;
        return deltaAccel;
    }

    Vector2 Arrive(Vector2 const  targetPosition,float deltaTime)
    {
        float r = 250;
        Vector2 desiredVelocity = targetPosition - m_fish->pos;

        float distance = Distance(targetPosition, m_fish->pos);


        if (distance < r)
        {
            float slowRatio = distance / r;
            if (distance < 15)
            {
                desiredVelocity = { 0,0 };
            }
            else {

            desiredVelocity = Normalize(desiredVelocity) *(m_maxSpeed*slowRatio) ;
            }
            Vector2 steering = (desiredVelocity)-m_fish->velo;
            steering = Normalize(steering) * m_maxAacceleration;
            m_fish->accel = m_fish->accel + steering;
            return steering;
        }
    }

    void Draw()
    {
        Vector2 veloNorm = Normalize(m_fish->velo);

        DrawCircleV(m_fish->pos, circleRadius, BLACK);
   //     DrawCircleLines(m_fish->pos.x, m_fish->pos.y, 150, RED);
        DrawLineV(m_fish->pos, m_fish->pos + veloNorm * 100, RED);
        DrawText(TextFormat("Speed: %f.1", Length(m_fish->velo)), 200, 400 + 45, 20, BLACK);

        for (int i = 0; i < whiskerCount; i++)
        {
            DrawLineV(m_fish->pos, m_fish->pos + whiskers[i], (detection[i]) ? RED : GREEN);

        }
    }

private:
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

};




int main(void)
{
    std::vector<Agent*> agents;
    Agent* fish1 = new Agent(100, 100, 100, 100, 300, 350);
    RigidBody* food1 = new RigidBody({ 100.0f, 100.0f }, {0,0}, {0,0}, {0,0}, 0.0f, 0.0f);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);

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

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);
        rlImGuiBegin();

        const float dt = GetFrameTime();
        mousePOS = GetMousePosition();




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

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {

            fish1->Seek(mousePOS, dt);
        }


        fish1->Arrive(mousePOS,dt);
            

        fish1->Update(dt);
        fish1->Draw();
       

    //    fish1->Avoid(position, dt, radius);




        position = WrapAroundScreen(position);
        DrawCircleV(mousePOS, radius, RED);
        DrawCircleV(position, 25, BLUE);
        DrawCircleV(food1->pos, 40, YELLOW);

        //    DrawLineV(position, position + Vector2{100,0}, BLACK);
        //    DrawLineV(position, position + direction *100, PINK);
        //    DrawLineV(position, position + wiskerLeft, (CheckCollisionLineCircle(position,mousePOS,wiskerLeft,radius)) ? RED : GREEN);
        //    DrawLineV(position, position + wiskerRight, GREEN);
        //    DrawCircleV(nearestPointC, 5, BLACK);


        //  DrawLineV(position, position + acceleration, RED);
        //  DrawLineV(position, position + desiredVelocity, PURPLE);



        timer += dt;
        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    return 0;

}