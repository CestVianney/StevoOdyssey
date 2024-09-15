#pragma once

#include "ECS.hpp"
#include "SDL.h"

class WallsComponent : public Component
{
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	WallsComponent() = default;

	~WallsComponent()
	{
		SDL_DestroyTexture(texture);
	}

	WallsComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale)
	{
		texture = TextureManager::LoadTexture("resources/tiles/wallTiles.png");

		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = tsize * tscale;
	}

	void update() override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};