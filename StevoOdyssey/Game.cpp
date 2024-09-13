#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "Manage.hpp"
#include "Level.hpp"
#include "LevelFactory.hpp"
#include "Level2020.cpp"
#include "Level2021.cpp"
#include "Level2121.cpp"
#include <iostream>

Map* map;
Manager Game::manager;
Level* currentLevel = nullptr;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
int currentLevelX = 20;
int currentLevelY = 20;

int mapWidth = 800;
int mapHeight = 640;

SDL_Rect Game::camera = { 0,0,mapWidth,mapHeight };

bool Game::isRunning = false;

Game::Game()
{}
Game::~Game()
{
	delete currentLevel;
}

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

	initLevels();

	player = &manager.addEntity();
	player->addComponent<TransformComponent>(500, 550, 32, 32, 1);
	player->addComponent<SpriteComponent>("resources/characters/stevo.png", true);
	player->addComponent<KeyboardController>();
	player->addComponent<ColliderComponent>("player");
	player->addGroup(Game::groupPlayers);

	loadLevel(20, 20);
}

void Game::loadLevel(int x, int y)
{
	std::string levelNumber = std::to_string(x) + std::to_string(y);
	if (currentLevel)
	{
		currentLevel->clean();
		delete currentLevel;
		currentLevel = nullptr;
	}

	currentLevel = LevelFactory::getInstance().createLevel(levelNumber);
	std::cout << "Loading level " << levelNumber << std::endl;
	currentLevel->init(manager, player);
}

void Game::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Game::isRunning = false;
			break;
		default:
			manager.handleEvent(event);  
			break;
		}
	}
}


void Game::update() 
{
	if (currentLevel != nullptr) {
		manager.refresh();
		currentLevel->update(manager, player);
		manager.update();
	}
	else {
		std::cerr << "currentLevel is null" << std::endl;
	}
	if (player->getComponent<TransformComponent>().position.x >= mapWidth) {
		currentLevelX += 1;
		loadLevel(currentLevelX, currentLevelY);
	}
	else if (player->getComponent<TransformComponent>().position.x <= 0) {
		currentLevelX -= 1;
		loadLevel(currentLevelX, currentLevelY);
	}
	else if (player->getComponent<TransformComponent>().position.y <= 0) {
		currentLevelY -= 1;
		loadLevel(currentLevelX, currentLevelY);
	}
	else if (player->getComponent<TransformComponent>().position.y >= mapHeight) {
		currentLevelY += 1;
		loadLevel(currentLevelX, currentLevelY);
	}
}


void Game::render() 
{

	SDL_RenderClear(renderer);
	currentLevel->render(manager, player);
	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	currentLevel->clean();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool Game::running()
{
	return isRunning;
}

void Game::initLevels()
{
	LevelFactory::getInstance().registerLevel("2020", []() { return new Level2020(); });
	LevelFactory::getInstance().registerLevel("2021", []() { return new Level2021(); });
	LevelFactory::getInstance().registerLevel("2121", []() { return new Level2121(); });
}