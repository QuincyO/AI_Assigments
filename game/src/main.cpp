#include "rlImGui.h"
#include "Math.h"
#include "TileMap.h"
#include "Pathfinder.h"
#include <iostream>
#include <vector>

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GAME3001 - Assignment 2");
    rlImGuiSetup(true);
    SetTargetFPS(60);
   
    Tilemap map("../game/assets/p1_walk01.png");
    map.player.SetSize(64, 64);
    
    float timer = 0;

    int wallChance = 20;
    map.RegnerateLevel(wallChance);
    
    Vector2 position = { SCREEN_WIDTH/2,SCREEN_HEIGHT/2 };//in px
    Vector2 velocity = { 0,0 }; //In px/s
    float maxSpeed = 10;
    float maxAccel = 150;
    Vector2 acceleration = { 0,0 }; //In px/s/s
    Vector2 direction = { 13.0f,25.0f };

    Vector2 playerPosition = map.Respawn();

    bool imGui = false;
    bool useGUI = false;
    bool showTextures = false;
    bool showNodes = false;
    bool showPath = true;

    Vector2 mousePOS = { 0,0 };

    Pathfinder pathfinder = Pathfinder();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
            rlImGuiBegin();

            if (IsKeyPressed(KEY_GRAVE)) imGui = !imGui;
            if (imGui)
            {
                ImGui::SliderInt("Wall%", &wallChance, 0, 100, NULL);
                if (ImGui::Button("Regenerate Level"))
                {
                    map.RegnerateLevel(wallChance);
                }
                if (ImGui::Button("Toggle Textures"))
                {
                    showTextures = !showTextures;
                }
                if (ImGui::Button("Toggle Node Visuals"))
                {
                    showNodes = !showNodes;
                }
                if (ImGui::Button("Toggle Pathfinder Visuals"))
                {
                    showPath = !showPath;
                }
            }

            //Pathfinding
            Vector2 mouseTilePos = map.ScreenPosToTilePos(GetMousePosition());
            if (map.IsInsideGrid(mouseTilePos))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && map.IsTraversible(mouseTilePos))
                {
                    pathfinder = Pathfinder(&map, playerPosition, mouseTilePos);
                }

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && map.IsTraversible(mouseTilePos))
                {
                    pathfinder = Pathfinder(&map, playerPosition, mouseTilePos);
                    pathfinder.SolvePath();
                }
            }
            if (pathfinder.map != nullptr)
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    pathfinder.ProcessNextIterationFunctional();
                }

                if (showPath) pathfinder.DrawCurrentState();
            }

            if (IsKeyPressed(KEY_W)) map.MoveSpriteUp();
            if (IsKeyPressed(KEY_S)) map.MoveSpriteDown();
            if (IsKeyPressed(KEY_A)) map.MoveSpriteLeft();
            if (IsKeyPressed(KEY_D)) map.MoveSpriteRight();
            
        const float dt = GetFrameTime();
        mousePOS = GetMousePosition();

        
        if (showTextures) map.DrawTextures(tileTexture);
        map.DrawTiles();
        map.DrawBorders();
        if (showNodes) map.DrawNodes();
        map.DrawSprite();

        timer += dt;
        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
   
    return 0;

}