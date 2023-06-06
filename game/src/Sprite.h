#pragma once
#include "rlImGui.h"

class Sprite
{
public:


	void CreateImage(const char* filepath)
	{
		Sprite::LoadImage(filepath);
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.width = texture.width;
		srcRect.height = texture.height;

		dstRect;
		dstRect;
		dstRect.width = srcRect.width;
		dstRect.height = srcRect.height;
	}

	void LoadImage(const char* filepath)
	{
		texture = LoadTexture(filepath);
	}

	Rectangle GetSource() { return srcRect; }
	Rectangle GetDest() { return dstRect; }

	Texture2D GetTexture() { return texture; }

protected:
	Rectangle srcRect, dstRect;
	Texture2D texture;
};