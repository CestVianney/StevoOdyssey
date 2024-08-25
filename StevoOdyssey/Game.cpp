#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include <iostream>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int mapWidth = 800;
int mapHeight = 640;

SDL_Rect Game::camera = { 0,0,800,640};

bool Game::isRunning = false;

auto& player(manager.addEntity());

SDL_Texture* backgroundTexture = nullptr; 


enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};

Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	} 

	map = new Map("resources/tiles/tiles.png", 1, 32);
	map->LoadMap("resources/maps/gravity.map",25,20);
	backgroundTexture = TextureManager::LoadTexture("resources/backgrounds/Paris.png");


	player.addComponent<TransformComponent>(mapWidth/2,0);
	player.addComponent<SpriteComponent>("resources/characters/stevo.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& preColliders(manager.getGroup(Game::groupPreColliders));
auto& colliders(manager.getGroup(Game::groupColliders));

void Game::handleEvents() 
{
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}


void Game::update() 
{

    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
	bool isPreColliders = false;
    manager.refresh();

	for (auto& c : preColliders)
	{
		SDL_Rect cCol = c->getComponent<PrecolliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().handlePreCollision(cCol);
			isPreColliders = true;
		}
	}

    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(cCol, playerCol))
        {
            player.getComponent<TransformComponent>().handleCollision(cCol, isPreColliders);
        }
    }
	player.getComponent<TransformComponent>().update(isPreColliders);
	manager.update();
    // Calcul de la position de la caméra uniquement sur l'axe X
    camera.x = player.getComponent<TransformComponent>().position.x - mapWidth / 2;

    // Limites de la caméra sur l'axe X
    if (camera.x < 0)
        camera.x = 0;
    if (camera.x > mapWidth - camera.w)
        camera.x = mapWidth - camera.w;
}


void Game::render() 
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}

	SDL_RenderPresent(renderer);

}
void Game::clean()
{
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool Game::running()
{
	return isRunning;
}

