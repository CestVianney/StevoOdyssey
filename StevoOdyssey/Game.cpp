#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "Manage.hpp"
#include "Level.hpp"
#include "LevelFactory.hpp"
#include "LevelLoader.hpp"
#include <iostream>

Map* map;
Manager Game::manager;
Level* currentLevel = nullptr;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
int currentLevelX = 11;
int currentLevelY = 12;

int mapWidth = 800;
int mapHeight = 640;

SDL_Rect Game::camera = { 0,0,mapWidth,mapHeight };

bool Game::isRunning = false;

Game::Game() : currentState(STATE_GAME)
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
		flags = SDL_WINDOW_OPENGL;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		SDL_GLContext glcontext = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, glcontext);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		font = TTF_OpenFont("resources/fonts/cour.ttf", 18);
		if (!font)
			std::cout << "Failed to load font" << std::endl;	

		isRunning = true;
	} 

	initLevels();

	player = &manager.addEntity();
	player->addComponent<TransformComponent>(500, 350, 32, 32, 1);
	player->addComponent<SpriteComponent>("resources/characters/stevo.png", true);
	player->addComponent<KeyboardController>();
	player->addComponent<ColliderComponent>("player");
	player->addGroup(Game::groupPlayers);

	loadLevel(currentLevelX, currentLevelY, 9);
}

SDL_Texture* Game::renderText(const char* message, SDL_Color color)
{
	SDL_Surface* surf = TTF_RenderText_Solid(font, message, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	return texture;
}

void Game::loadLevel(int x, int y, int exitOrigin)
{
	std::string levelNumber = std::to_string(x) + std::to_string(y);
	TextureManager::ClearTextures();
	if (currentLevel)
	{
		currentLevel->clean(manager);
		delete currentLevel;
		currentLevel = nullptr;
		switch (exitOrigin)
		{
		case 1: // Top
			player->getComponent<TransformComponent>().position.y = mapHeight - 48;
			break;
		case 2: // Right
			player->getComponent<TransformComponent>().position.x = 0 + 16;
			break;
		case 3: // Bottom
			player->getComponent<TransformComponent>().position.y = 0 + 16;
			break;
		case 4: // Left
			player->getComponent<TransformComponent>().position.x = mapWidth - 48;
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
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_p) {
				if (currentState == STATE_GAME)
					currentState = STATE_MENU;
				else if (currentState == STATE_MENU)
					currentState = STATE_GAME;
			}
		default:
			manager.handleEvent(event);  
			break;
		}
	}
}


void Game::update()
{
	if (currentState == STATE_GAME) {
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


		if (currentLevel != nullptr) {
			manager.refresh();
			currentLevel->update(manager, player);
			manager.update();
		}
	}
}


void Game::render() 
{
	switch (currentState)
	{
	case STATE_GAME:
		renderGame();
		break;
	case STATE_MENU:
		renderMenu();
		break;
	default:
		break;
	}

}

void Game::renderGame()
{
	SDL_RenderClear(renderer);
	currentLevel->render(manager, player);
	SDL_RenderPresent(renderer);
}

void Game::renderMenu()
{
	SDL_Color white = { 255, 255, 255, 255 };

	SDL_Texture* resumeTexture = renderText("Reprendre (Appuyez sur P)", white);
	SDL_Rect resumeRect = { 200, 200, 400, 50 };  
	SDL_RenderCopy(renderer, resumeTexture, NULL, &resumeRect);
	SDL_DestroyTexture(resumeTexture);  

	// Afficher l'option "Quitter"
	SDL_Texture* quitTexture = renderText("Quitter (Appuyez sur Q)", white);
	SDL_Rect quitRect = { 200, 300, 400, 50 };  
	SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);
	SDL_DestroyTexture(quitTexture);

	SDL_RenderPresent(renderer);  
}

void Game::clean()
{
	currentLevel->clean(manager);
	TTF_CloseFont(font);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
}

bool Game::running()
{
	return isRunning;
}

void Game::initLevels()
{	
	LevelLoader::initLevels();
}