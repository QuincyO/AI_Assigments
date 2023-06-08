#include "GameObject.h"

GameObject::GameObject(Texture2D texture, Vector2 position, int width, int height)
{
	this->texture = texture;
	this->position = position;
	this->width = width;
	this->height = height;
}