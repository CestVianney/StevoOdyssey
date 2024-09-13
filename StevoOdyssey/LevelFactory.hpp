#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>
#include "Level.hpp"

class LevelFactory {
public:
    static LevelFactory& getInstance() {
        static LevelFactory instance;
        return instance;
    }

    void registerLevel(const std::string& levelName, std::function<Level* ()> constructor) {
        levelRegistry[levelName] = constructor;
        std::cout << "Registered level: " << levelName << std::endl;
    }

    Level* createLevel(const std::string& levelName) {
        auto it = levelRegistry.find(levelName);
        if (it != levelRegistry.end()) {
            std::cout << "Creating level: " << levelName << std::endl;
            return it->second();  
        }
        else {
            std::cerr << "Level not found: " << levelName << std::endl;
            return nullptr;
        }
    }

private:
    LevelFactory() {}
    LevelFactory(const LevelFactory&) = delete;
    void operator=(const LevelFactory&) = delete;

    std::unordered_map<std::string, std::function<Level* ()>> levelRegistry;
};