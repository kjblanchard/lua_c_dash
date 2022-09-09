#pragma once
#include <stdlib.h>

struct lua_State;
struct GraphicsDevice;

typedef struct World{
    struct lua_State* global_lua_state_ptr;
    struct GraphicsDevice* graphics;
    unsigned char is_running;

} World;

World* GameWorld;
World* CreateWorld();
int InitializeSdl();
void DestroyWorld(World* world);
