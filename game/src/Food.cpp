#include "Food.h"

void Food::Damage()
{
	if (!ate)
	{
		foodPoints =foodPoints -100 * eatSpeed;
		if (foodPoints <= 0)ate = true;
	}

}

void Food::Draw() 
{
	dst.x = GetPosition().x;
	dst.y = GetPosition().y;
	std::string healthToDisplay = std::to_string((int)foodPoints);
	char const* charHealthToDisplay = healthToDisplay.c_str();
	Vector2 origin = { dst.width / 2,dst.height / 2 };

	DrawTexturePro(TextureManager::GetTexture("food"), src, dst, origin, 0, WHITE);

	DrawText(charHealthToDisplay, dst.x + 20, dst.y + 20, 50, PURPLE);
}

