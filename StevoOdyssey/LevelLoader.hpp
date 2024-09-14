#pragma once

#include "LevelFactory.hpp"

#include "Level2020.cpp"
#include "Level2021.cpp"
#include "Level2121.cpp"

class LevelLoader {
public:
	static void initLevels() {
		LevelFactory::getInstance().registerLevel("2020", []() {return new Level2020(); });
		LevelFactory::getInstance().registerLevel("2021", []() {return new Level2021(); });
		LevelFactory::getInstance().registerLevel("2121", []() {return new Level2121(); });
	}
};