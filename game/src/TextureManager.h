#pragma once
#include <map>
#include <string>
#include "rlImGui.h"

class TextureManager
{
public:
	static Texture2D* Load(const char* path, const std::string key);
	static void Unload(const std::string key);
	static Texture2D* GetTexture(const std::string key);
	static void Quit();

private:
	static std::map<std::string, Texture2D*> s_textures;
	TextureManager() = default;
};

