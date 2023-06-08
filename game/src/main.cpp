#include "rlImGui.h"
#include "Math.h"
#include <iostream>
#include <vector>
#include "Agent.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//        position = position + (m_fish->GetVelocity() * deltaTime) + ((acceleration * 0.5f) * deltaTime * deltaTime);
//        position = WrapAroundScreen(position);
//        m_fish->SetPosition(position);
//        m_fish->SetVelocity(m_fish->GetVelocity() + acceleration * deltaTime);









int main(void)
{
    std::vector<Agent*> agents;
    Agent* fish1 = new Agent({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }, 100, 100, 300, 350);
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




        //  ImGui::SliderFloat2("position", &(position.x), 0, SCREEN_WIDTH);
        //  ImGui::SliderFloat2("velocity", &(velocity.x), -maxSpeed, maxSpeed);
        //  ImGui::SliderFloat2("Acceleration", &(acceleration.x), -maxAccel, maxAccel);
        //  ImGui::SliderFloat("Max Acceleration", &maxAccel, 1, 1500); 
        //  ImGui::SliderFloat("Max Speed", &maxSpeed, -1, 1500);



       
            

       



        position = WrapAroundScreen(position);
        DrawCircleV(mousePOS, radius, RED);
        DrawCircleV(position, 25, BLUE);

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