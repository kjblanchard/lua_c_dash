#include <assert.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include "SDL2/SDL_video.h"
#include "base/point.h"
#include "core/graphics_device.h"
#include "core/debug.h"
#include "core/world.h"
#include "sound/sound.h"


static void ProcessInput()
{
    SDL_Event sdlEvent;
    ProcessDebugWindowInputBegin();
    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        if(sdlEvent.window.windowID != GameWorld->graphics->window_id)
        {
            ProcessDebugWindowInput(&sdlEvent);
            continue;
        }
        //Handle this windows events
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
    ProcessDebugWindowInputEnd();
}



int main()
{
    World* world = CreateWorld();
    assert(world);
    //Uint64 previous = SDL_GetTicks64();
    //double lag = 0.0;
    int result = InitDebugWindow();
    assert(result);
    SDL_RaiseWindow(GameWorld->graphics->game_window);
    InitializeSound();
    PlayBgm(0);
    while (world->is_running) 
    {
        //Uint64 current = SDL_GetTicks64();
        //Uint64 elapsed = current - previous;
        //previous = current;
        ProcessInput();
        UpdateSound();
        ProcessDebugWindowGraphics();
        SDL_SetRenderDrawColor(world->graphics->game_renderer, 255,255,255,255);
        SDL_RenderClear(world->graphics->game_renderer);
        SDL_RenderPresent(world->graphics->game_renderer);
    } 
    ShutdownDebugWindow();
    SDL_DestroyRenderer(world->graphics->game_renderer);
    SDL_DestroyWindow(world->graphics->game_window);
    CloseSound();
    SDL_Quit();
    return 0;
}



