#include "rlImGui.h"
#include "Math.h"
#include "TileMap.h"
#include "Pathfinder.h"
#include "Agent2.h"
#include <iostream>
#include <vector>

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GAME3001 - Assignment 2");
    rlImGuiSetup(true);
    SetTargetFPS(60);

    Tilemap* map = new Tilemap("../game/assets/p1_walk01.png");
    Agent* player = new Agent({ 0,0,0,0 }, { 0,0,0,0 }, map, "../game/assets/goomba.png");
    map->player.SetSize(64, 64);

    float timer = 0;

    int wallChance = 20;
    map->RegnerateLevel(wallChance);

    Vector2 position = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };//in px
    Vector2 velocity = { 0,0 }; //In px/s
    float maxSpeed = 10;
    float maxAccel = 150;
    Vector2 acceleration = { 0,0 }; //In px/s/s
    Vector2 direction = { 13.0f,25.0f };

    TileCoord playerPosition = map->Respawn();

    bool imGui = true;
    bool useGUI = false;
    bool showTextures = false;
    bool showNodes = false;
    bool showPath = true;
    bool firstSelected = false;
    bool secondSelected = false;

    Vector2 mousePOS = { 0,0 };


    TileCoord startNode = { 0,0 };
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
            ImGui::Text("End Coord: %d : %d,", endNode.x, endNode.y);
            ImGui::SliderInt("Wall%", &wallChance, 0, 100, NULL);
            if (ImGui::Button("Regenerate Level"))
            {
                map->RegnerateLevel(wallChance);
                startNode = endNode = {};
                firstSelected = false;
                secondSelected = false;
                pathfinder.Restart();
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
        map->DrawTiles();
        map->DrawBorders();



        TileCoord mouseTilePos = map->ScreenPosToTilePos(GetMousePosition());
        if (map->IsInsideGrid(mouseTilePos))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                pathfinder = Pathfinder(map, player->m_tilePosition, TileCoord(mouseTilePos));
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                pathfinder = Pathfinder(map, player->m_tilePosition, TileCoord(mouseTilePos));
                pathfinder.SolvePath();
            }
        }
        if (pathfinder.map != nullptr)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                pathfinder.ProcessNextIterationFunctional();
            }
            if (showNodes) map->DrawNodes();
            if (showPath) pathfinder.DrawCurrentState();
        }
        if (pathfinder.IsCompleted())
        {
            player->MoveAlongCompletedPath(pathfinder.GetSolution(), player->GetTilePosition(), pathfinder.goalNode, dt);
        }

        Texture2D tileTexture;
        player->DrawAgent();

        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();

    return 0;

}