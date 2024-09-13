// Collision.hpp
#pragma once
#include <SDL.h>
#include "ColliderComponent.hpp"

enum class CollisionType {
    None,
    TOP,
    BOT,
    LEFT,
    RIGHT
};

class Collision {
public:
    static CollisionType AABB(const SDL_Rect& recA, const SDL_Rect& recB);
    static CollisionType AABB(const ColliderComponent& colA, const ColliderComponent& colB);
    static bool isNear(const SDL_Rect& recA, const SDL_Rect& recB, int distance);
};
