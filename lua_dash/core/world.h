#pragma once
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>

struct lua_State;
struct GraphicsDevice;

typedef struct World{
    struct lua_State* global_lua_state_ptr;
    struct GraphicsDevice* graphics;
    unsigned char is_running;
    Mix_Music* playing_music;

} World;

World* GameWorld;
World* CreateWorld();
int InitializeSdl();
void DestroyWorld(World* world);
