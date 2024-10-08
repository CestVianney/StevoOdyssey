#include "Level.hpp"
#include "TextureManager.hpp"
#include "Collision.hpp"
#include "Manage.hpp"

class Level2121 : public Level {
public:
	void init(Manager& manager, Entity* player) override {
		map = new Map("resources/tiles/tiles.png", 1, 32);
		map->LoadMap("resources/maps/2121.map", 25, 20);
	}
};