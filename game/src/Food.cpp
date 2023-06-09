#include "Food.h"

void Food::Damage()
{
	if (!ate)
	{
		foodPoints -= 1 * eatSpeed;
		if (foodPoints <= 0)ate = true;
	}

}

