#pragma once
#include "rlImGui.h"
#include "TextureManager.h"

class Sprite
{
public:
	Sprite(std::string key)
	{
		Texture2D temp = TextureManager::GetTexture(key);

		src.x = 0;
		src.y = 0;
		src.width = temp.width;
		src.height = temp.height;

		dst.x = 0;
		dst.y = 0;
		dst.width = src.width;
		dst.height = src.height;

	}


protected:
	Rectangle src,dst;

};