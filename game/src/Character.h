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


class Character : public SpriteObject
{
public:
	TileCoord m_tilePosition;

	Character(Rectangle source, Rectangle dest, Tilemap* map, const char* assetFilePath, float moveSpeed = 64)
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

		m_tilePosition = { 0, 0 };

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

	TileCoord Move(TileCoord position)
	{
		TileCoord temp;
		temp.x = position.x;
		temp.y = position.y;

		if (IsKeyPressed(KEY_W))
		{
			if (map->IsTraversible({ temp.x, temp.y - 1 }))
				temp.y -= 1;
		}

		if (IsKeyPressed(KEY_S))
		{
			if (map->IsTraversible({ temp.x, temp.y + 1 }))
				temp.y += 1;
		}

		if (IsKeyPressed(KEY_A))
		{
			if (map->IsTraversible({ temp.x - 1, temp.y }))
				temp.x -= 1;
		}

		if (IsKeyPressed(KEY_D))
		{
			if (map->IsTraversible({ temp.x + 1, temp.y }))
				temp.x += 1;
		}

		return temp;
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

	void DrawCharacter()
	{
		m_tilePosition.x = map->ScreenPosToTilePos(m_dest.x).x;
		m_tilePosition.y = map->ScreenPosToTilePos(m_dest.y).y;
		DrawTexturePro(m_texture, m_source, m_dest, { 0,0 }, 0, WHITE);
	}

	
private:
	Texture2D m_texture;
	Tilemap* map;
	float moveSpeed;
};