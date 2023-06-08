#include "Food.h"

int Food::getFoodPoints()
{
	return foodPoints;
}

void Food::eatFood(int eatSpeed, float deltaTime)
{
	foodPoints = foodPoints - eatSpeed * deltaTime;
}