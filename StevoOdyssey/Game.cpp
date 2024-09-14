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

bool skipCollisionCheck = false;
int framesToSkip = 1;

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

	loadLevel(20, 20, 9);
}

void Game::loadLevel(int x, int y, int exitOrigin)
{
	std::string levelNumber = std::to_string(x) + std::to_string(y);
	if (currentLevel)
	{
		currentLevel->clean(manager);
		delete currentLevel;
		currentLevel = nullptr;
		switch (exitOrigin)
		{
		case 1: // Top
			player->getComponent<TransformComponent>().position.y = mapHeight - 64;
			break;
		case 2: // Right
			player->getComponent<TransformComponent>().position.x = 0 + 32;
			break;
		case 3: // Bottom
			player->getComponent<TransformComponent>().position.y = 0 + 32;
			break;
		case 4: // Left
			player->getComponent<TransformComponent>().position.x = mapWidth - 64;
			break;
		default:
			break;
		}
	}
	player->update();
	currentLevel = LevelFactory::getInstance().createLevel(levelNumber);
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
	if (skipCollisionCheck) {
		framesToSkip--;
		if (framesToSkip <= 0) {
			skipCollisionCheck = false; 
		}
	}
	else {
		bool rightMapchange = player->getComponent<TransformComponent>().position.x >= mapWidth - 32;
		bool leftMapchange = player->getComponent<TransformComponent>().position.x <= 0;
		bool topMapchange = player->getComponent<TransformComponent>().position.y <= 0;
		bool bottomMapchange = player->getComponent<TransformComponent>().position.y >= mapHeight - 32;


		if (rightMapchange) {
			currentLevelX += 1;
			loadLevel(currentLevelX, currentLevelY, 2);
		}
		else if (leftMapchange) {
			currentLevelX -= 1;
			loadLevel(currentLevelX, currentLevelY, 4);
		}
		else if (topMapchange) {
			currentLevelY -= 1;
			loadLevel(currentLevelX, currentLevelY, 1);
		}
		else if (bottomMapchange) {
			currentLevelY += 1;
			loadLevel(currentLevelX, currentLevelY, 3);
		}
	}

	if (currentLevel != nullptr) {
		manager.refresh();
		currentLevel->update(manager, player);
		manager.update();
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
	currentLevel->clean(manager);
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