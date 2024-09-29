#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include "ECS.hpp"
#include <vector>

class ColliderComponent;

enum GameState {
	STATE_GAME,
	STATE_MENU
};

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void loadLevel(int x, int y, int exitOrigin);

	void handleEvents();
	void update();
	void render();
	void clean();
	bool running();
	
	void initLevels();

	void renderGame();
	void renderMenu();

	static Manager manager;
	static SDL_Renderer *renderer; 
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupNpcs,
		groupWalls,
		groupProps,
		groupPlants,
		groupColliders
	};

private:
	int cnt = 0;
	SDL_Window* window; 
	Entity* player;

	GameState currentState;
};