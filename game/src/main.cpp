#include "rlImGui.h"
#include "Math.h"
#include "TileMap.h"
#include "Pathfinder.h"
#include "Character.h"
#include <iostream>
#include <vector>
#include <time.h>

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GAME3001 - Assignment 2");
    rlImGuiSetup(true);
    SetTargetFPS(60);
    srand(time(NULL));
    
    Tilemap* map = new Tilemap();

    //Tile textures
    Texture2D tileTexture = LoadTexture("../game/assets/tiles.png");
    
    //Monster
    Character* monster = new Character({ 0,0,0,0 }, { 0,0,0,0 }, map, "../game/assets/minotaur.png");
    map->player.SetSize(64, 64);

    //Player
    TileCoord playerPosition = { 0, 0 };
    Rectangle playerDstRec = { playerPosition.x, playerPosition.y, 64, 64 };
    Rectangle playerScrRec = { 0, 0, 15, 16 };
    Texture2D playerTexture = LoadTexture("../game/assets/link.png");

    float timer = 0;

    int wallChance = 20;
    map->RegnerateLevel(wallChance);

    Vector2 position = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };//in px
    Vector2 velocity = { 0,0 }; //In px/s
    float maxSpeed = 10;
    float maxAccel = 150;
    Vector2 acceleration = { 0,0 }; //In px/s/s
    Vector2 direction = { 13.0f,25.0f };

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

        //PLAYER MOVEMENT
        playerPosition = map->Move(playerPosition);

        //DRAW MAP
        map->DrawTiles();
        map->DrawBorders();
        if (showTextures) map->DrawTextures(tileTexture);
        if (showNodes) map->DrawNodes();

        //PATHFINDING
        TileCoord mouseTilePos = map->ScreenPosToTilePos(GetMousePosition());
        if (map->IsInsideGrid(mouseTilePos))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                pathfinder = Pathfinder(map, monster->m_tilePosition, TileCoord(mouseTilePos));
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                pathfinder = Pathfinder(map, monster->m_tilePosition, TileCoord(mouseTilePos));
                pathfinder.SolvePath();
                monster->m_tilePosition = TileCoord(mouseTilePos);
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
        if (pathfinder.IsCompleted())
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                monster->MoveAlongCompletedPath(pathfinder.GetSolution(), monster->GetTilePosition(), pathfinder.goalNode, dt);
            }
        }


        //DRAW CHARACTERS
        monster->DrawCharacter();

        playerDstRec.x = map->TilePosToScreenPos(playerPosition).x;
        playerDstRec.y = map->TilePosToScreenPos(playerPosition).y;
        DrawTexturePro(playerTexture, playerScrRec, playerDstRec, { 0, 0 }, 0.0f, WHITE);

        //ImGUI
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

            rlImGuiEnd();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;

}