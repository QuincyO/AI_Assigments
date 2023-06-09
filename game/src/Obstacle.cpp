#include "Obstacle.h"

void Obstacle::Draw()
{
	dst.x = GetPosition().x;
	dst.y = GetPosition().y;

	Vector2 origin = { dst.width / 2,dst.height / 2 };

	DrawTexturePro(TextureManager::GetTexture("rock"), src, dst, origin, 0, WHITE);
}