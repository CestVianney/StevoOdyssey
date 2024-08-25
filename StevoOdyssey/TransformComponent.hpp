#pragma once

#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include <algorithm>

class TransformComponent : public Component {

public:
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 19;
	int scale = 2;
	int speed = 3;

	float gravity = 0.5f;
	float velocityY = 0.0f;
	bool falling = true;

	TransformComponent() 
	{
		position.Zero();
	}

	TransformComponent(int sc)
	{
		position.x = 400;
		position.y = 320;
		scale = sc;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override {
		velocity.Zero();
	}

	void update(bool isPreCollider) {

		if (!isPreCollider) {
			velocityY += gravity; 
			position.y += velocityY;
		}

		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

	void handlePreCollision(SDL_Rect precollider) {
		gravity = 0.0f;
		bool isBelowTile = position.y + height * scale > precollider.y + precollider.h;
		bool isInTile = position.y + height * scale < precollider.y + precollider.h;
		if (isInTile && !isBelowTile) {
			int difference = static_cast<int> (std::abs(position.y - precollider.y));
			position.y = std::min((int)position.y + (difference / 10), precollider.y - precollider.h - 1);
		}
	}

	void handleCollision(SDL_Rect collider, bool isPrecollision) {

		bool isCollisionPlafond = position.y < collider.y + collider.h;
		bool isCollisionDroite = position.x + width * scale >= collider.x && position.x + width * scale < collider.x + 20;
		bool isCollisionGauche = position.x < collider.x + collider.w && position.x < collider.x + collider.w - 10;

		if (isPrecollision) {
			if (isCollisionDroite) {
				position.x = collider.x - width * scale;
			}
			else if (isCollisionGauche) {
				std::cout << "Collistion à gauche !" << std::endl;
				position.x = collider.x + width * scale;
			}
		}
	}
};