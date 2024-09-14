#pragma once

#include "Game.hpp"
#include "Components.hpp"
#include "Manage.hpp"
#include "Map.hpp"

class Level {
public:
	virtual ~Level() {}
	
	virtual void init(Manager& manager, Entity* player) = 0;
	virtual void update(Manager& manager, Entity* player) {
		try {
			SDL_Rect playerCol = player->getComponent<ColliderComponent>().collider;
			Manage::manageCollisions(manager, player, playerCol);
			Manage::manageInteractions(manager, player, playerCol);
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in Level1::update: " << e.what() << std::endl;
		}
	};
	virtual void render(Manager& manager, Entity* player) {
		try {
			Manage::manageRendering(manager);
		}
		catch (const std::exception& e) {
			std::cerr << "Failed Manage::render" << e.what() << std::endl;
		}
	};
	virtual void clean(Manager& manager) {
		if (map) {
			delete map;
			map = nullptr;
		}
		Manage::cleanGroups(manager);
		manager.refresh();
	};

protected:
	Map* map = nullptr;
};