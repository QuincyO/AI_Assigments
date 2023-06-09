#pragma once
#include "raylib.h"

class ModeManager
{
private:
	ModeManager();
	
	

	int currentMode;
	
public:
	void ChangeMode();
	void ModeUpdate(int currentMode);
};