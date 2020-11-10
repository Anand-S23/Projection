#include <SDL.h>
#include <Primitive.h>
#include <math.h> 
#include <vector>
#include <stdint.h>

#include "projection.h"

global bool Running = true; 

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("3D Projection",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED, 
                                           WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer)
        {
            uint32_t last_tick_time = 0;
            uint32_t delta = 0;

            while(Running)
            {
                uint32_t tick_time = SDL_GetTicks();
                delta = tick_time - last_tick_time;
                last_tick_time = tick_time;

                SDL_Event event;
                while(SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        Running = false; 
                    }
                }
            }
        }
        else 
        {
            printf("Could not create window: %s\n", SDL_GetError());
            return 1;
        }
    }
    else 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Quit(); 
    return 0;
}