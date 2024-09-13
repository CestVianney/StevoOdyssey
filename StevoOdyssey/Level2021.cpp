#include "Level.hpp"
#include "TextureManager.hpp"
#include "Collision.hpp"
#include "Manage.hpp"

class Level2021: public Level {
public:
	void init(Manager& manager, Entity* player) override {
		map = new Map("resources/tiles/tiles.png", 1, 32);
		map->LoadMap("resources/maps/2021.map", 25, 20);
		player->getComponent<TransformComponent>().position = Vector2D(100, 550);

	}

	void update(Manager& manager, Entity* player) override {
		try {
			SDL_Rect playerCol = player->getComponent<ColliderComponent>().collider;
			Manage::manageCollisions(manager, player, playerCol);
			Manage::manageInteractions(manager, player, playerCol);
			Manage::drawArea(*player, 50);
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in Level1::update: " << e.what() << std::endl;
		}
	}

	void render(Manager& manager, Entity* player) override {
		try {
			Manage::manageRendering(manager);
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in Level1::render: " << e.what() << std::endl;
		}
	}

	void clean() override {
		if (map) {
			std::cout << "Cleaning Level2021" << std::endl;
			delete map;
			map = nullptr;
		}
	}
};