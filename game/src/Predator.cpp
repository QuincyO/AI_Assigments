#include "Predator.h"

void Predator::Draw()
{
	dst.x = GetPosition().x;
	dst.y = GetPosition().y;

	Vector2 origin = { dst.width / 2,dst.height / 2 };

	DrawTexturePro(TextureManager::GetTexture("enemy"), src, dst, origin, 0, WHITE);
}