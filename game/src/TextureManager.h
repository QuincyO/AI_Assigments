#pragma once
#include <map>
#include <string>
#include "raylib.h"



class TextureManager
{
public:
	std::map<std::string, Texture2D> TextureManager::textures;
public:
	TextureManager() = default;

	Texture2D Load(const char* filepath, const std::string key)
	{
		Texture2D texture = LoadTexture(filepath);

		textures.emplace(key, texture);

		return texture;
	}

	void Unload(const std::string key)
	{
		UnloadTexture(textures[key]);
		textures.erase(key);
	}

	Texture2D GetTexture(const std::string key)
	{
		return textures[key];
	}

	void Quit()
	{
		for (auto i : textures)
		{
			UnloadTexture(textures[i.first]);
		}
		textures.clear();
	}

};
