#include "Manage.hpp"
#include "Collision.hpp"
#include "Utils.hpp"
#include "Level.hpp"

void Manage::manageCollisions(Manager& manager, Entity* player, SDL_Rect playerCol) {
	for (auto& c : manager.getGroup(Game::groupColliders))
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		CollisionType collisionType = Collision::AABB(playerCol, cCol);

		if (collisionType == CollisionType::TOP)
		{
			player->getComponent<TransformComponent>().position.y = cCol.y - cCol.h;
		}
		if (collisionType == CollisionType::BOT)
		{
			player->getComponent<TransformComponent>().position.y = cCol.y + cCol.h;
		}
		if (collisionType == CollisionType::LEFT)
		{
			player->getComponent<TransformComponent>().position.x = cCol.x - cCol.w;
		}
		if (collisionType == CollisionType::RIGHT)
		{
			player->getComponent<TransformComponent>().position.x = cCol.x + cCol.w;
		}
	}

	for (auto& c : manager.getGroup(Game::groupPlayers))
	{
		TransformComponent& cPos = c->getComponent<TransformComponent>();
		if (cPos.position.x < 0)
		{
			cPos.position.x = 0;
		}
		else if (cPos.position.x + cPos.width > 800)
		{
			cPos.position.x = 800 - cPos.width;
		}
		if (cPos.position.y < 0)
		{
			cPos.position.y = 0;
		}
		else if (cPos.position.y + cPos.height > 640)
		{
			cPos.position.y = 640 - cPos.height;
		}
	}
}

void Manage::manageInteractions(Manager& manager, Entity* player, SDL_Rect playerCol) {
	for (auto& e : manager.getGroup(Game::groupNpcs))
	{
		TransformComponent& ePos = e->getComponent<TransformComponent>();
		SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
		if (Collision::isNear(playerCol, eCol, 50))
		{
			
		}
	}
}

void Manage::drawArea(Entity& entity, int radius) {
	TransformComponent& ePos = entity.getComponent<TransformComponent>();
	int centerX = ePos.position.x + ePos.width / 2;
	int centerY = ePos.position.y + ePos.height / 2;
	SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255); // Couleur rouge
	SDL_RenderDrawCircle(Game::renderer, centerX, centerY, radius);
}

void Manage::manageRendering(Manager& manager) {
	auto& tiles = manager.getGroup(Game::groupMap);
	auto& players = manager.getGroup(Game::groupPlayers);
	auto& enemiesGroup = manager.getGroup(Game::groupEnemies);
	auto& npcs = manager.getGroup(Game::groupNpcs);
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemiesGroup)
	{
		e->draw();
	}
	for (auto& n : npcs)
	{
		n->draw();
	}
}

void Manage::initNpc(Manager& manager, int x, int y) {
	auto& npc(manager.addEntity());
	npc.addComponent<TransformComponent>(x, y);
	npc.addComponent<SpriteComponent>("resources/characters/lino.png");
	npc.addComponent<ColliderComponent>("npc");
	npc.addGroup(Game::groupNpcs);
}

void Manage::cleanGroups(Manager& manager) {
	auto& colliders = manager.getGroup(Game::groupColliders);
	for (auto& entity : colliders) {
		entity->destroy(); 
	}
	colliders.clear();

	auto& map = manager.getGroup(Game::groupMap);
	for (auto& entity : map) {
		entity->destroy();
	}
	map.clear();

	auto& enemies = manager.getGroup(Game::groupEnemies);
	for (auto& entity : enemies) {
		entity->destroy();
	}
	enemies.clear();

	auto& npcs = manager.getGroup(Game::groupNpcs);
	for (auto& entity : npcs) {
		entity->destroy();
	}
	npcs.clear();
}