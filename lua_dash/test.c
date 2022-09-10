#include <string.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_timer.h"
#include "base/Point.h"
#include "core/GraphicsDevice.h"
#include "core/world.h"
#include "sound/openal.h"


static void ProcessInput()
{
    SDL_Event sdlEvent;
    puts("Hello");
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
    StreamPlayer* player =  play("build/finish.ogg");
    while (world->is_running) 
    {
        Uint64 current = SDL_GetTicks64();
        Uint64 elapsed = current - previous;
        printf("The elapsed time is %lld",elapsed);
        previous = current;
        ProcessInput();
        update(player);
        SDL_RenderPresent(world->graphics->game_renderer);
    }
    close(player);
    SDL_Quit();
    return 0;
}



