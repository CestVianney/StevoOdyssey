#include "Level.hpp"
#include "TextureManager.hpp"
#include "Collision.hpp"
#include "Manage.hpp"

class Level1110 : public Level {
public:
	void init(Manager& manager, Entity* player) override {
		map = new Map("resources/tiles/ground.png", 1, 32);
		map->LoadMap("resources/maps/1110.map", 25, 20);
	}
};