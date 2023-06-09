#pragma once
#include "rlImGui.h"
#include "TextureManager.h"

class Sprite
{
public:
	Sprite(std::string key)
	{
		Texture2D* temp = TextureManager::GetTexture(key);
	}


protected:
	Rectangle sourceTransform;

};