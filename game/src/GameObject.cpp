#include "GameObject.h"

GameObject::GameObject(Vector2 position,int hp, int width, int height)
{
	this->hp = hp;
	this->position = position;
	this->width = width;
	this->height = height;
}