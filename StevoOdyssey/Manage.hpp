#pragma once

#include <SDL.h>
#include "Components.hpp"

class Manage
{
public:
	static void manageCollisions(Manager& manager, Entity* player, SDL_Rect playerCol);
	static void manageInteractions(Manager& manager, Entity* player, SDL_Rect playerCol);
	static void drawArea(Entity& entity, int radius);
	static void manageRendering(Manager& manager);
	static void initNpc(Manager& manager, int x, int y);
	static void cleanGroups(Manager& manager);

private:
	SDL_Renderer* renderer;
};