#pragma once
#include <vector>
#include "rlImGui.h"
#include "TileCoord.h"
#include "raylib.h"
#include "Sprite.h"
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 800
#define MAP_WIDTH (int)(1280/64)
#define MAP_HEIGHT (int)(720/64)

using namespace std;

enum class Tile 
{
	Floor = 0, //Floor tiles can be walked on
	Wall, //Wall tiles cannot be walked on
	Count

};

class Tilemap
{

public:
	Tilemap(const char* filepath) : player(filepath) {}
	Tilemap() {}
private:


	Tile tiles[MAP_WIDTH][MAP_HEIGHT]; //Standard array cannot be chaged at runtime

	const TileCoord NORTH = { 0,-1 };
	const TileCoord SOUTH = { 0,1 };
	const TileCoord EAST = { 1,0 };
	const TileCoord WEST = { -1,0 };

	int width = 0;
	int height = 0;
vector<vector<Tile>> v_Tiles; //Dynamic Array that can be changed during Runtime

public:
	Sprite player;

	int tileSizeX = 64;
	int tileSizeY = 64;

public:
	int GetWidth()const { return width; }
	int GetHeight() const { return height; }

	int GetGridWidth() {return MAP_WIDTH; } //Get Number of columns in the grid
	int GetGridHeight() { return MAP_HEIGHT; } //Get Number of rows in the grid
	Tile GetTile(int x, int y); //Get the tile at the specified coordinate in the grid
	Tile GetTile(TileCoord tilePosition) ; //Get the tile at the specified coordinate in the grid
	void SetTile(int x, int y, Tile type); //St the tile at the specified coordinate in the grid
	bool IsInsideGrid(int x, int y); //Returns true of the coordinate is insdie the grid, false otherwise
	bool IsInsideGrid(TileCoord tilePosition); //Returns true of the coordinate is insdie the grid, false otherwise
	Vector2 TilePosToScreenPos(TileCoord tilePosition); //Convert from a tile coordinate to a screen position
	Vector2 TilePosToScreenPos(int x, int y); //Convert from a tile coordinate to a screen positon
	Vector2 ScreenPosToTilePos(Vector2 postionOnScreen);//Find a tile coordinate given a position on the screen over a tile
	
	bool IsTraversible(TileCoord tilePosition);
	std::vector<TileCoord> GetAllTraversibleTiles();

	void RegnerateLevel(int chanceofWall = 50);

	Vector2 GetTileCenter(TileCoord tilePosition);

	void MoveSpriteUp( );
	void MoveSpriteLeft();
	void MoveSpriteDown();
	void MoveSpriteRight();

	TileCoord Respawn();
	void ReplacePlayer();

	void DrawTiles();
	void DrawBorders(Color color = BLACK);
	void DrawNodes();
	void DrawSprite();
	void DrawTextures(Texture2D texture);
	

	std::vector<TileCoord> GetTraversibleTilesAdjacentTo(TileCoord tilePosition);

};