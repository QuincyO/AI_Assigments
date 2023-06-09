#include "TextureManager.h"
#include <iostream>

std::map<std::string, Texture2D*> TextureManager::s_textures;

Texture2D* TextureManager::Load(const char* path, const std::string key)
{

	Texture2D* temp = &LoadTexture(path);
		s_textures.emplace(key, temp); //Added TO the Map
		std::cout << "Successfully Loaded Texture: " << key << std::endl;

		return temp;
}

void TextureManager::Unload(const std::string key)
{
	if (s_textures.find(key) != s_textures.end())
	{
		UnloadTexture(*s_textures[key]);
		s_textures.erase(key);
	}
	else
	{
		std::cout << "Could not Unload: " << key << std::endl;
	}
}

Texture2D* TextureManager::GetTexture(const std::string key)
{
	return s_textures[key];
}

void TextureManager::Quit()
{
	for (auto const& image : s_textures)
	{
		UnloadTexture(*s_textures[image.first]);
	}
	s_textures.clear();
}