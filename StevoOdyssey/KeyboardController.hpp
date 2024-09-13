#pragma once

#include "Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent* sprite;
    bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;
    SDL_Keycode lastHorizontalKey = SDLK_UNKNOWN;
    SDL_Keycode lastVerticalKey = SDLK_UNKNOWN;


	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_z:  // Haut
                moveUp = true;
                lastVerticalKey = SDLK_z;
                break;
            case SDLK_q:  // Gauche
                moveLeft = true;
                lastHorizontalKey = SDLK_q;
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_d:  // Droite
                moveRight = true;
                lastHorizontalKey = SDLK_d;
                sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            case SDLK_s:  // Bas
                moveDown = true;
                lastVerticalKey = SDLK_s;
                break;
            default:
                break;
            }
        }

        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_z:  // Haut
                moveUp = false;
                if (lastVerticalKey == SDLK_z)
                {
                    lastVerticalKey = moveDown ? SDLK_s : SDLK_UNKNOWN;
                }
                break;
            case SDLK_q:  // Gauche
                moveLeft = false;
                if (lastHorizontalKey == SDLK_q)
                {
                    lastHorizontalKey = moveRight ? SDLK_d : SDLK_UNKNOWN;
                }
                break;
            case SDLK_d:  // Droite
                moveRight = false;
                if (lastHorizontalKey == SDLK_d)
                {
                    lastHorizontalKey = moveLeft ? SDLK_q : SDLK_UNKNOWN;
                }
                break;
            case SDLK_s:  // Bas
                moveDown = false;

                if (lastVerticalKey == SDLK_s)
                {
                    lastVerticalKey = moveUp ? SDLK_z : SDLK_UNKNOWN;
                }
                break;
            case SDLK_ESCAPE:  // Échapper pour quitter
                Game::isRunning = false;
                break;
            default:
                break;
            }
        }

        // Mettre à jour la vélocité en fonction des touches pressées pour les mouvements horizontaux
        if (lastHorizontalKey == SDLK_q && moveLeft)
        {
            transform->velocity.x = -1;
            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        }
        else if (lastHorizontalKey == SDLK_d && moveRight)
        {
            transform->velocity.x = 1;
            sprite->spriteFlip = SDL_FLIP_NONE;
        }
        else
        {
            transform->velocity.x = 0;
        }

        // Mettre à jour la vélocité en fonction des touches pressées pour les mouvements verticaux
        if (lastVerticalKey == SDLK_z && moveUp)
        {
            transform->velocity.y = -1;
        }
        else if (lastVerticalKey == SDLK_s && moveDown)
        {
            transform->velocity.y = 1;
        }
        else
        {
            transform->velocity.y = 0;
        }

        // Mettre à jour l'animation en fonction du mouvement
        if (moveUp || moveDown || moveLeft || moveRight)
        {
            sprite->Play("Walk");
        }
        else
        {
            sprite->Play("Idle");
        }
    }

    void handleEvent(SDL_Event& event) override
    {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_z:
                moveUp = true;
                lastVerticalKey = SDLK_z;
                break;
            case SDLK_q:
                moveLeft = true;
                lastHorizontalKey = SDLK_q;
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_d:
                moveRight = true;
                lastHorizontalKey = SDLK_d;
                sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            case SDLK_s:
                moveDown = true;
                lastVerticalKey = SDLK_s;
                break;
            default:
                break;
            }
        }

        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_z:
                moveUp = false;
                if (lastVerticalKey == SDLK_z) {
                    lastVerticalKey = moveDown ? SDLK_s : SDLK_UNKNOWN;
                }
                break;
            case SDLK_q:
                moveLeft = false;
                if (lastHorizontalKey == SDLK_q) {
                    lastHorizontalKey = moveRight ? SDLK_d : SDLK_UNKNOWN;
                }
                break;
            case SDLK_d:
                moveRight = false;
                if (lastHorizontalKey == SDLK_d) {
                    lastHorizontalKey = moveLeft ? SDLK_q : SDLK_UNKNOWN;
                }
                break;
            case SDLK_s:
                moveDown = false;
                if (lastVerticalKey == SDLK_s) {
                    lastVerticalKey = moveUp ? SDLK_z : SDLK_UNKNOWN;
                }
                break;
            case SDLK_ESCAPE:
                Game::isRunning = false;
                break;
            default:
                break;
            }
        }
    }
};