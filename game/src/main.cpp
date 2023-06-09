#include "rlImGui.h"
#include "Math.h"
#include <iostream>
#include <vector>
#include "Agent.h"
#include "GameObject.h"
#include "Food.h"
#include "Obstacle.h"
#include "Predator.h"
#include "Steeringbehaviors.h"
#include <map>
#include "TextureManager.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GAME3001 - Assignment 1");
    Image icon = LoadImage("../game/assets/Fish/fish.png");
    SetWindowIcon(icon);
    rlImGuiSetup(true);
    SetTargetFPS(60);
    srand(time(NULL));

    TextureManager::Load("../game/assets/Fish/fish.png", "fish");
    TextureManager::Load("../game/assets/Fish/food.png", "food");
    TextureManager::Load("../game/assets/Fish/enemy.png", "enemy");
    TextureManager::Load("../game/assets/Fish/obstacle.png", "rock");
    
    //Creating bools for GameMode
    std::map<std::string, bool> Mode;
    Mode.emplace("Off", false);
    Mode.emplace("Seek", false);
    Mode.emplace("Arrive", false);
    Mode.emplace("Flee", false);
    Mode.emplace("Avoid", false);

    std::vector<GameObject*> objects;
    std::vector<Agent*> agents;

    //Randomizing TopSpeed and Accleration Of fish
    for (int i = 0; i < 15; i++)
    {
        Vector2 tempPos = {
            rand() % SCREEN_WIDTH + 1,
            rand() % SCREEN_HEIGHT + 1,
        };

        int randomMaxSpeed = 250 + (rand() % 151);
        int randomMaxAccel = 250 + (rand() % 151);

        if (i < 5) { randomMaxSpeed += 150; randomMaxAccel += 150; }
        else if (i >=10) { randomMaxSpeed -= 150; randomMaxAccel -= 150; }



        agents.push_back(new Agent(tempPos, 100, 100, randomMaxAccel, randomMaxSpeed,"fish"));
    }

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
        bool seek = false;
        bool flee = false;
        bool arrive = false;

    while (!WindowShouldClose())
    {
        Vector2 tempAccel = { 0,0 };
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

        //****SET MODE****
        if (IsKeyPressed(KEY_ONE))
        {
            for (auto& options : Mode)
            {
                options.second = false;
            }
            Mode["Seek"] = true;
        }
        if (IsKeyPressed(KEY_TWO))
        {
            for (auto& options : Mode)
            {
                options.second = false;
            }
            Mode["Flee"] = true;
        }
        if (IsKeyPressed(KEY_THREE))
        {
            for (auto& options : Mode)
            {
                options.second = false;
            }
            Mode["Arrive"] = true;
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            for (auto& options : Mode)
            {
                options.second = false;
            }
            Mode["Avoid"] = true;
        }
        if (IsKeyPressed(KEY_ZERO))
        {
            for (auto& options : Mode)
            {
                options.second = false;
            }
        }

        //****MODES****
        if (Mode["Seek"])
        {
            DrawText("Seek Mode", 15, 20, 50, WHITE);
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                for (Agent* fish : agents)
                {
                    Steeringbehaviors::Seek(fish, mousePOS);
                }

            }
        }
        else if (Mode["Arrive"])
        {
            DrawText("Arrive Mode", 15, 20, 50, WHITE);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                objects.push_back(new Food(mousePOS, 100,"food"));
            }
        }
        else if (Mode["Flee"])
        {

            DrawText("Flee Mode", 15, 20, 50, WHITE);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                objects.push_back(new Predator(mousePOS,"enemy"));
            }
        }
        else if (Mode["Avoid"])
        {
            DrawText("Avoid Mode", 15, 20, 50, WHITE);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                objects.push_back(new Obstacle(mousePOS,"rock"));
            }
        }

        //****UPDATE AGENTS****
        for (Agent* fish : agents)
        {

            for (GameObject* object : objects)
            {
                float distance = Distance(fish->GetPosition(), object->GetPosition());

                if (distance < fish->Neighborhood.slowingRadius)
                {
                    if (object->GetType() == ObjectType::FoodType)
                    {
                    fish->Neighborhood.inHood = true;
                        //Arrive to Food
                        Steeringbehaviors::Arrive(fish, object);
                        if (distance <= fish->Neighborhood.arrivedRadius)
                        {
                            object->Damage();
                            if (object->IsAte())
                            {
                                object->RemoveFromScreen();
                            }
                        }
                    }
                     if (object->GetType() == ObjectType::ObstacleType)
                        {
                        fish->Neighborhood.inHood = true;
                        //Avoid Obstacle
                        Steeringbehaviors::Avoid(fish, object->GetPosition(), dt);
                        }
                     if (object->GetType() == ObjectType::Enemy)
                     {
                         fish->Neighborhood.inHood = true;
                         Steeringbehaviors::Flee(fish, object->GetPosition());
                     }
                }
                else fish->Neighborhood.inHood = false;
            }

        }

        //****DRAW****
        for (GameObject* object : objects)
        {
            object->Draw();
        }

        for (Agent* fish : agents)
        {

            fish->Draw();
            fish->Update(dt);
        }
       
        if (IsKeyPressed(KEY_SPACE))
        {
            for (GameObject* object : objects)
            {
                delete object;
                object = nullptr;
                objects.clear();
            }
        }


        DrawCircleV(mousePOS, radius, RED);

        timer += dt;
        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    return 0;

}