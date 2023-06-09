#include "rlImGui.h"
#include "Math.h"
#include <iostream>
#include <vector>
#include "Agent.h"
#include "GameObject.h"
#include "Food.h"
#include "Obstacle.h"
#include "Steeringbehaviors.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//        position = position + (m_fish->GetVelocity() * deltaTime) + ((acceleration * 0.5f) * deltaTime * deltaTime);
//        position = WrapAroundScreen(position);
//        m_fish->SetPosition(position);
//        m_fish->SetVelocity(m_fish->GetVelocity() + acceleration * deltaTime);





class Circle : public GameObject
{
public:

    void Draw()
    {
        DrawCircleV(position, radius, PURPLE);
    }

    ObjectType GetType() const { return ObjectType::Food; }

    Circle(Vector2 position, int hp, int width, int height) : GameObject(position,hp, 50, 50), health{ hp } {}
    float radius = 40;
    int health;
};

class Rock : public GameObject
{
public:
    void Draw()
    {
        DrawCircleV(position, 40, BLACK);
    }
    Rock(Vector2 position,int hp,int width,int height) : GameObject(position,hp, 50, 50){}

    ObjectType GetType() const { return ObjectType::Obstacle; }
};


int main(void)
{
    std::vector<GameObject*> objects;
    
    Circle* circle = new Circle({ 600,500 },50, 50, 50);
    Rock* rock = new Rock({ 250,250 },50, 50, 50);

    objects.push_back(circle);
    objects.push_back(rock);
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
        if (IsKeyPressed(KEY_ONE))
        {
            seek = true;
            flee = false;
            arrive = false;

        }
        if (IsKeyPressed(KEY_TWO))
        {
            flee = true;
            seek = false;
        }
        if (IsKeyPressed(KEY_THREE))
        {
            arrive = true;
            seek = false;
            flee = false;
        }
        if (IsKeyPressed(KEY_ZERO))
        {
            seek = false;
            flee = false;
            arrive = false;
        }



        if (seek && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            tempAccel = Steeringbehaviors::Seek(fish1, mousePOS);
            //fish1->Update(Steeringbehaviors::Seek(fish1, mousePOS),dt);
        }
        else if (arrive && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            
            objects.push_back(new Circle(mousePOS, 50, 50, 50));
        }
        
        for (GameObject* object : objects)
        {
            float distance = Distance(fish1->GetPosition(), object->GetPosition());

            if (distance < fish1->Neighborhood.slowingRadius)
            {
                if (object->GetType() == ObjectType::Food)
                {
                    //Arrive to Food
                    tempAccel = tempAccel + Steeringbehaviors::Arrive(fish1, object);
                    //fish1->Update(Steeringbehaviors::Arrive(fish1,object), dt);

                }
                else if (object->GetType() == ObjectType::Obstacle)
                {
                    //Avoid Obstacle
                    Steeringbehaviors::Avoid(fish1, object->GetPosition(), dt);

                }
                else if (object->GetType() == ObjectType::Enemy)
                {
                    //Flee Predator
                   // tempAccel = Steeringbehaviors::Flee(fish1)
                }
            }
        }



        fish1->Update(tempAccel,dt);
        fish1->Draw();


        for (GameObject* object : objects)
        {
            object->Draw();
        }

       



        DrawCircleV(mousePOS, radius, RED);



        timer += dt;
        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    return 0;

}