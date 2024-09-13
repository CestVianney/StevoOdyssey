#pragma once

#include "Game.hpp"
#include "Components.hpp"
#include "Map.hpp"

class Level {
public:
	virtual ~Level() {}
	
	virtual void init(Manager& manager, Entity* player) = 0;
	virtual void update(Manager& manager, Entity* player) = 0;
	virtual void render(Manager& manager, Entity* player) = 0;
	virtual void clean() = 0; 

protected:
	Map* map = nullptr;
};