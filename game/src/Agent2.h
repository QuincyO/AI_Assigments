#pragma once
#include "rlImGui.h"
#include "TileCoord.h"
#include "TileMap.h"


class SpriteObject
{
public:
	SpriteObject(Rectangle source, Rectangle dest)
		:m_source{ source },
		m_dest{ dest }
	{

	}



protected:
	Rectangle m_source;
	Rectangle m_dest;
};


class Agent : public SpriteObject
{
public:
	Agent(Rectangle source, Rectangle dest, Tilemap* map, const char* assetFilePath, float moveSpeed = 64)
		:SpriteObject(source, dest),
		map{ map },
		moveSpeed{ moveSpeed }
	{
		m_texture = LoadTexture(assetFilePath);
		m_source.width = m_texture.width;
		m_source.height = m_texture.height;

		m_dest.x = map->TilePosToScreenPos(0, 0).x;
		m_dest.y = map->TilePosToScreenPos(0, 0).y;
		m_dest.width = 64;
		m_dest.height = 64;


	}
	void SetPosition(Vector2 tilePosition)
	{
		m_dest.x = tilePosition.x;
		m_dest.y = tilePosition.y;
	}

	TileCoord  GetTilePosition()
	{
		return m_tilePosition;
	}
	Vector2 GetScreenPosition()
	{
		return map->TilePosToScreenPos(m_tilePosition);
	}

	void MoveRight()
	{
		TileCoord tempCoord = m_tilePosition;
		tempCoord.x += 1;
		if (map->IsTraversible(tempCoord))
		{
			SetPosition(map->TilePosToScreenPos(tempCoord));
		}
	}
	void MoveLeft()
	{
		TileCoord tempCoord = m_tilePosition;
		tempCoord.x -= 1;
		if (map->IsTraversible(tempCoord))
		{
			SetPosition(map->TilePosToScreenPos(tempCoord));
		}
	}
	void MoveUp()
	{
		TileCoord tempCoord = m_tilePosition;
		tempCoord.y -= 1;
		if (map->IsTraversible(tempCoord))
		{
			SetPosition(map->TilePosToScreenPos(tempCoord));
		}
	}
	void MoveDown()
	{
		TileCoord tempCoord = m_tilePosition;
		tempCoord.y += 1;
		if (map->IsTraversible(tempCoord))
		{
			SetPosition(map->TilePosToScreenPos(tempCoord));
		}
	}


	void MoveAlongCompletedPath(std::list<TileCoord> path, TileCoord startNode, TileCoord endNode, float deltaTime)
	{
		float moveDuration = 0.5f;

		for (const auto& tile : path)
		{
			Vector2 screenPosition = map->TilePosToScreenPos(tile);

			Vector2 distance = screenPosition - GetScreenPosition();
			moveDuration = Length(distance) / moveSpeed;

			float elapsedTime = 0.0f;
			while (elapsedTime < moveDuration)
			{
				elapsedTime += deltaTime;

				float t = elapsedTime / moveDuration;

				Vector2 interpolatedPosition = Lerp(GetScreenPosition(), screenPosition, t);

				SetPosition(interpolatedPosition);
			}

			SetPosition(screenPosition);
		}
	}


	void DrawAgent()
	{
		m_tilePosition.x = map->ScreenPosToTilePos(m_dest.x).x;
		m_tilePosition.y = map->ScreenPosToTilePos(m_dest.y).y;
		DrawTexturePro(m_texture, m_source, m_dest, { 0,0 }, 0, WHITE);
	}

	TileCoord m_tilePosition;
private:
	Texture2D m_texture;
	Tilemap* map;
	float moveSpeed;
};