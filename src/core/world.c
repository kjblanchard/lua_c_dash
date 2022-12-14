#include <SDL2/SDL.h>
#include <lauxlib.h>
#include "world.h"
#include "graphics_device.h"

World *GameWorld = NULL;

World *CreateWorld()
{
    if (GameWorld)
        return GameWorld;
    World *world_ptr = NULL;
    if (world_ptr != NULL)
        printf("dead");
    world_ptr = (World *)malloc(sizeof(World));
    lua_State *L = luaL_newstate();
    world_ptr->global_lua_state_ptr = L;
    GameWorld = world_ptr;
    InitializeSdl();
    world_ptr->graphics = CreateGraphicsDevice();
    world_ptr->is_running = 1;
    return world_ptr;
}

int InitializeSdl()
{
    int sdl_video_init_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    if (sdl_video_init_result != 0)
    {
        return 0;
        printf("Error loading sdl, %s", SDL_GetError());
    }
    return 1;
}

void DestroyWorld(World *world)
{
    lua_close(world->global_lua_state_ptr);
    world->global_lua_state_ptr = NULL;
    free(world);
    GameWorld = NULL;
}
