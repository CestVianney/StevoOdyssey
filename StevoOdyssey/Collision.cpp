#include "Collision.hpp"
#include "ColliderComponent.hpp"

CollisionType Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
    if (
        recA.x + recA.w >= recB.x &&
        recB.x + recB.w >= recA.x &&
        recA.y + recA.h >= recB.y &&
        recB.y + recB.h >= recA.y
        )
    {
        // Déterminer le type de collision
        int overlapTop = recA.y + recA.h - recB.y;
        int overlapBot = recB.y + recB.h - recA.y;
        int overlapLeft = recA.x + recA.w - recB.x;
        int overlapRight = recB.x + recB.w - recA.x;

        int minOverlap = std::min({ overlapTop, overlapBot, overlapLeft, overlapRight });

        if (minOverlap == overlapTop)
		{
			return CollisionType::TOP;
		}
        if (minOverlap == overlapBot)
            {
            return CollisionType::BOT;
			}
        if (minOverlap == overlapLeft)
			{
			return CollisionType::LEFT;
            }
		if (minOverlap == overlapRight)
            {
            return CollisionType::RIGHT;
			}
        }
    return CollisionType::None;
}

CollisionType Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
    return AABB(colA.collider, colB.collider);
}

bool Collision::isNear(const SDL_Rect& recA, const SDL_Rect& recB, int distance)
{
    int centerAX = recA.x + recA.w / 2;
    int centerAY = recA.y + recA.h / 2;
    int centerBX = recB.x + recB.w / 2;
    int centerBY = recB.y + recB.h / 2;

    int deltaX = centerAX - centerBX;
    int deltaY = centerAY - centerBY;
    int distanceBetweenCenters = std::sqrt(deltaX * deltaX + deltaY * deltaY);

    return distanceBetweenCenters <= distance;
}