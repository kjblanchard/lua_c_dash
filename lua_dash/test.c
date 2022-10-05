#include <assert.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_video.h"
#include "base/point.h"
#include "core/graphics_device.h"
#include "core/debug.h"
#include "core/world.h"
#include "sound/sound.h"

static unsigned int debug_window_enabled = 0;
static unsigned int debug_window_created = 0;

static void ProcessInput()
{
    SDL_Event sdlEvent;
    if(debug_window_created)
        ProcessDebugWindowInputBegin();
    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        if(debug_window_created && debug_window_enabled && sdlEvent.window.windowID != GameWorld->graphics->window_id)
        {
            if(sdlEvent.type == SDL_KEYDOWN && (sdlEvent.key.keysym.sym == SDLK_BACKQUOTE || sdlEvent.key.keysym.sym == SDLK_ESCAPE))
            {
                debug_window_enabled = (debug_window_enabled) ? 0 : 1;
                ToggleDebugWindow(debug_window_enabled);
            }
            else if (sdlEvent.type == SDL_WINDOWEVENT)
            {
                if(sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    debug_window_enabled = (debug_window_enabled) ? 0 : 1;
                    ToggleDebugWindow(debug_window_enabled);

                }
            }
            else
                ProcessDebugWindowInput(&sdlEvent);
            continue;
        }
        if (sdlEvent.type == SDL_KEYDOWN)
        {
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                GameWorld->is_running = 0;
            }
            else if (sdlEvent.key.keysym.sym == SDLK_BACKQUOTE)
            {
                debug_window_enabled = (debug_window_enabled) ? 0 : 1;
                ToggleDebugWindow(debug_window_enabled);
            }
        }
        else if (sdlEvent.type == SDL_QUIT)
            GameWorld->is_running = 0;
    }
    if(debug_window_created)
        ProcessDebugWindowInputEnd();
}



int main(int argc, char **argv)
{
    World* world = CreateWorld();
    assert(world);
    InitializeDebugLogFile();
    //Uint64 previous = SDL_GetTicks64();
    //double lag = 0.0;
    debug_window_created = InitDebugWindow();
    SDL_RaiseWindow(GameWorld->graphics->game_window);
    ToggleDebugWindow(0);
    InitializeSound();
    PlayBgm(0, 0.3f);
    while (world->is_running) 
    {
        //Uint64 current = SDL_GetTicks64();
        //Uint64 elapsed = current - previous;
        //previous = current;
        ProcessInput();
        UpdateSound();
        if(debug_window_created && debug_window_enabled)
            ProcessDebugWindowGraphics();
        SDL_SetRenderDrawColor(world->graphics->game_renderer, 255,255,255,255);
        SDL_RenderClear(world->graphics->game_renderer);
        SDL_RenderPresent(world->graphics->game_renderer);
    } 
    if(debug_window_created)
        ShutdownDebugWindow();

    SDL_DestroyRenderer(world->graphics->game_renderer);
    SDL_DestroyWindow(world->graphics->game_window);
    CloseDebugLogFile();
    CloseSound();
    DestroyGraphicsDevice(world->graphics);
    DestroyWorld(world);
    SDL_Quit();
    return 0;
}



