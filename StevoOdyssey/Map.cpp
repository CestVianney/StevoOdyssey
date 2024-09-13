#include "Map.hpp"
#include "Game.hpp"
#include <fstream>
#include "ECS.hpp"
#include "Components.hpp"
#include "Level.hpp"

Map::Map(const char* mfp, int ms, int ts) : mapFilePath(mfp), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

Map::~Map() {
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    char c;
    std::fstream mapFile;
    mapFile.open(path);

    int srcX, srcY;

    for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
			mapFile.ignore();
		}
	}
	
	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '2')
			{
				auto& tileColl(Game::manager.addEntity());
				tileColl.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tileColl.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}

	
	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(Game::manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, mapFilePath);
	tile.addGroup(Game::groupMap);
}
