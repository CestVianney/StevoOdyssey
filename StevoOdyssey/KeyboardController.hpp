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
            //case SDLK_s:  // Bas
            //    moveDown = true;
            //    lastVerticalKey = SDLK_s;
            //    break;
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
            //case SDLK_s:  // Bas
            //    moveDown = false;
            //    if (lastVerticalKey == SDLK_s)
            //    {
            //        lastVerticalKey = moveUp ? SDLK_z : SDLK_UNKNOWN;
            //    }
            //    break;
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
        //else if (lastVerticalKey == SDLK_s && moveDown)
        //{
        //    transform->velocity.y = 1;
        //}
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
        // if left shift isnt pressed, set speed to 3
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_LSHIFT] == 0)
		{
			transform->speed = 3;
        }
        else {
            transform->speed = 5;
        }
    }
};