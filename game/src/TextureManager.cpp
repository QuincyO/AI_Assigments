#include "TextureManager.h"
#include <iostream>

std::map<std::string, Texture2D> TextureManager::s_textures;

void TextureManager::Load(const char* path, const std::string key)
{


		s_textures.emplace(key, LoadTexture(path)); //Added TO the Map
		std::cout << "Successfully Loaded Texture: " << key << std::endl;


}

void TextureManager::Unload(const std::string key)
{
	if (s_textures.find(key) != s_textures.end())
	{
		UnloadTexture(s_textures[key]);
		s_textures.erase(key);
	}
	else
	{
		std::cout << "Could not Unload: " << key << std::endl;
	}
}

Texture2D TextureManager::GetTexture(const std::string key)
{
	return s_textures[key];
}

void TextureManager::Quit()
{
	for (auto const& image : s_textures)
	{
		UnloadTexture(s_textures[image.first]);
	}
	s_textures.clear();
}