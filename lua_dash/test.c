#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include "base/Point.h"
#include "core/GraphicsDevice.h"
#include "core/world.h"
#include "sound/sound.h"


static void ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        if (sdlEvent.type == SDL_KEYDOWN)
        {
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                GameWorld->is_running = 0;
            }
        }
        else if (sdlEvent.type == SDL_QUIT)
            GameWorld->is_running = 0;
    }
}



int main()
{
    World* world = CreateWorld();
    Uint64 previous = SDL_GetTicks64();
    double lag = 0.0;
    ProcessInput();
    InitializeSound();
    //StreamPlayer* player =  play("build/assets/victory2.ogg");
    PlayBgm(0);
    while (world->is_running) 
    {
        Uint64 current = SDL_GetTicks64();
        Uint64 elapsed = current - previous;
        previous = current;
        ProcessInput();
        UpdateSound();
        SDL_RenderPresent(world->graphics->game_renderer);
    }
    CloseSound();
    SDL_Quit();
    return 0;
}



