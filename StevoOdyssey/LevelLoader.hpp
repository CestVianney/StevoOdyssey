#pragma once

#include "LevelFactory.hpp"

#include "Level1010.cpp"
#include "Level1011.cpp"
#include "Level1012.cpp"
#include "Level1110.cpp"
#include "Level1111.cpp"
#include "Level1112.cpp"
#include "Level1210.cpp"
#include "Level1211.cpp"
#include "Level1212.cpp"

class LevelLoader {
public:
	static void initLevels() {
		LevelFactory::getInstance().registerLevel("1010", []() {return new Level1010(); });
		LevelFactory::getInstance().registerLevel("1011", []() {return new Level1011(); });
		LevelFactory::getInstance().registerLevel("1012", []() {return new Level1012(); });
		LevelFactory::getInstance().registerLevel("1110", []() {return new Level1110(); });
		LevelFactory::getInstance().registerLevel("1111", []() {return new Level1111(); });
		LevelFactory::getInstance().registerLevel("1112", []() {return new Level1112(); });
		LevelFactory::getInstance().registerLevel("1210", []() {return new Level1210(); });
		LevelFactory::getInstance().registerLevel("1211", []() {return new Level1211(); });
		LevelFactory::getInstance().registerLevel("1212", []() {return new Level1212(); });
	}
};