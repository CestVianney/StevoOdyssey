#pragma once
#include <SDL.h>
#include <cmath>

inline void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x1 = (radius - 1);
    int32_t y1 = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x1 >= y1)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, x + x1, y - y1);
        SDL_RenderDrawPoint(renderer, x + x1, y + y1);
        SDL_RenderDrawPoint(renderer, x - x1, y - y1);
        SDL_RenderDrawPoint(renderer, x - x1, y + y1);
        SDL_RenderDrawPoint(renderer, x + y1, y - x1);
        SDL_RenderDrawPoint(renderer, x + y1, y + x1);
        SDL_RenderDrawPoint(renderer, x - y1, y - x1);
        SDL_RenderDrawPoint(renderer, x - y1, y + x1);

        if (error <= 0)
        {
            ++y1;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x1;
            tx += 2;
            error += (tx - diameter);
        }
    }
}