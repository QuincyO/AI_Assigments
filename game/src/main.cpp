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

    int wallChance = 0;
    map.RegnerateLevel(wallChance);
    
    Vector2 position = { SCREEN_WIDTH/2,SCREEN_HEIGHT/2 };//in px
    Vector2 velocity = { 0,0 }; //In px/s
    float maxSpeed = 10;
    float maxAccel = 150;
    Vector2 acceleration = { 0,0 }; //In px/s/s
    Vector2 direction = { 13.0f,25.0f };

    TileCoord playerPosition = map.Respawn();

    bool imGui = true;
    bool useGUI = false;
    bool showTextures = false;
    bool showNodes = false;
    bool showPath = true;
    bool firstSelected = true;
    bool secondSelected = false;

    Vector2 mousePOS = { 0,0 };


    TileCoord startNode = {0,0};
    TileCoord endNode = {};
    Pathfinder pathfinder = Pathfinder();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
            rlImGuiBegin();

            const float dt = GetFrameTime();
            mousePOS = GetMousePosition();

            if (IsKeyPressed(KEY_GRAVE)) imGui = !imGui;
            if (imGui)
            {

                ImGui::Text("Start Coord: %d : %d,", startNode.x, startNode.y);
                ImGui::Text("Start Coord: %d : %d,", endNode.x, endNode.y);
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

            if (!firstSelected)
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    startNode = map.ScreenPosToTilePos(GetMousePosition());
                    firstSelected = true;
                }
            }
            else if (!secondSelected)
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    endNode = map.ScreenPosToTilePos(GetMousePosition());
                    secondSelected = true;
                }
            }
            else
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                {
                    startNode = endNode = {};
                    firstSelected = secondSelected = false;
                }
            }
            

            if (IsKeyPressed(KEY_SPACE))
            {
                pathfinder = Pathfinder(&map, startNode, endNode);
                pathfinder.SolvePath();
            }


            //Pathfinding
            Vector2 mouseTilePos = map.ScreenPosToTilePos(GetMousePosition());

            //if (map.IsInsideGrid(mouseTilePos))
            //{
            //    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && map.IsTraversible(mouseTilePos))
            //    {
            //        pathfinder = Pathfinder(&map, playerPosition, mouseTilePos);
            //    }

            //    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && map.IsTraversible(mouseTilePos))
            //    {
            //        pathfinder = Pathfinder(&map, playerPosition, mouseTilePos);
            //        pathfinder.SolvePath();
            //    }
            //}
            //if (pathfinder.map != nullptr)
            //{
            //    if (IsKeyPressed(KEY_SPACE))
            //    {
            //        pathfinder.ProcessNextIterationFunctional();
            //    }

            //    if (showPath) pathfinder.DrawCurrentState();
            //}


            


        Texture2D tileTexture;
       // if (showTextures) map.DrawTextures(tileTexture);
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