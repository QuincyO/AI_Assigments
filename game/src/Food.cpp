#include "Food.h"

void Food::Damage()
{
	if (!ate)
	{
		foodPoints =foodPoints -10 * eatSpeed;
		if (foodPoints <= 0)ate = true;
	}

}

void Food::Draw() 
{
	dst.x = GetPosition().x;
	dst.y = GetPosition().y;

	Vector2 origin = { dst.width / 2,dst.height / 2 };

	DrawTexturePro(TextureManager::GetTexture("food"), src, dst, origin, 0, WHITE);
}

