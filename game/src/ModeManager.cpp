#include "ModeManager.h"
#include <iostream>

ModeManager::ModeManager()
	: currentMode(0)
{

}

void ModeManager::ChangeMode()
{
	if (IsKeyPressed(KEY_ZERO)) //Default mode
	{
		currentMode = 0;
		std::cout << "Default mode selected" << std::endl;
	}
	if (IsKeyPressed(KEY_ONE)) //Seek mode
	{
		currentMode = 1;
		std::cout << "Default mode selected" << std::endl;
	}
	if (IsKeyPressed(KEY_TWO)) //Predator mode
	{
		currentMode = 2;
	}
	if (IsKeyPressed(KEY_THREE)) //Food mode
	{
		currentMode = 3;
	}
	if (IsKeyPressed(KEY_FOUR)) //Obstacle modex
	{
		currentMode = 4;
	}
}

void ModeManager::ModeUpdate(int currentMode)
{
	switch (currentMode)
	{
	case 0: 
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			//nothing
		}
		break;

	case 1:
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			//Seek mode
		}
		break;

	case 2:
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			//Place predators
		}
		break;

	case 3:
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			//Place food
		}
		break;

	case 4:
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			//Place obstacle
		}
		break;

	default:
		break;
	}
}