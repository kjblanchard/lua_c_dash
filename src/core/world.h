/**
 * @file world.h
 * @brief Holds the world struct so that we can reference things without polluting the global namespace.
 * @author Kevin Blanchard
 * @version 0.1
 * @date 2022-09-17
 */
#pragma once
#include <stdlib.h>

struct lua_State;
struct GraphicsDevice;

/**
 * @brief World contains the graphics and the global lua state that can be referenced by anyone.
 */
typedef struct World{
    struct lua_State* global_lua_state_ptr;
    struct GraphicsDevice* graphics;
    unsigned char is_running;

} World;

/**
 * @brief Pointer to the Gameworld.
 */
extern World* GameWorld;
/**
 * @brief Creates the gameworld, and initializes the global gameworkd pointer.
 *
 * @return 
 */
World* CreateWorld();
/**
 * @brief Loads SDL and it's components.
 *
 * @return 
 */
int InitializeSdl();
/**
 * @brief Destroy the world.
 *
 */
void DestroyWorld(World* world);
