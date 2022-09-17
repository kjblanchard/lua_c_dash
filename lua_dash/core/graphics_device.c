#include <stdlib.h>
#include <SDL2/SDL.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "graphics_device.h"
#include "SDL2/SDL_error.h"
#include "world.h"

static short int graphics_loaded = 0;
/**
 * @brief Read the config.lua file and get the values of the world and window height, and set them accordingly
 *
 * @param state Pointer to an open lua state
 * @param graphics Pointer to the Graphics Device that we load the data into
 */
static void load_graphics_config(lua_State* state, GraphicsDevice* graphics)
{
    //First we load the file chunk with load file, then we use a protectedcall to actually run it.  We print an error if it doesn't work.
    if(luaL_loadfile(state, "./build/config.lua") || lua_pcall(state, 0, 0, 0))
        printf("Error, cannot run config file");

    lua_getglobal(state, "Graphics");
    if(!lua_istable(state, -1))
    {
        printf("This isn't a table");
        exit(2);
    }
    //Gets the field from the last thing on the stack(which was the Graphics table above) with the string key.
    //We do have to increment our index for where the table it is getting the field from though.
    lua_getfield(state,-1,"world_width");
    graphics->world_size.x = lua_tonumber(state, -1);
    lua_getfield(state,-2,"world_height");
    graphics->world_size.y = lua_tonumber(state, -1);
    lua_getfield(state,-3,"window_height");
    graphics->window_size.y = lua_tonumber(state, -1);
    lua_getfield(state,-4,"window_width");
    graphics->window_size.x = lua_tonumber(state, -1);
    //Pop the 4 values we had just added, and also the table.
    lua_pop(state,5);

    printf("Window size X:%d Y:%d GameX: %d Y:%d",graphics->window_size.x, graphics->window_size.y, graphics->world_size.x, graphics->world_size.y);


}

GraphicsDevice* CreateGraphicsDevice()
{
    if(graphics_loaded)
        return NULL;
    ++graphics_loaded;
    GraphicsDevice* graphics = (GraphicsDevice*) malloc(sizeof(GraphicsDevice));
    load_graphics_config(GameWorld->global_lua_state_ptr, graphics);
    graphics->game_window = SDL_CreateWindow(
            "CLuaDash!",
            0,
            0,
            graphics->window_size.x,
            graphics->window_size.y,
            0);
    if (!graphics->game_window)
        printf("Error making window, %s",SDL_GetError());
    SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
    graphics->game_renderer = SDL_CreateRenderer(graphics->game_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!graphics->game_renderer)
        printf("Error making renderer %s",SDL_GetError());
        /* scale the renderer output for High-DPI displays This should be a integer and not a float. */
    {
        int render_w, render_h;
        int window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(graphics->game_renderer, &render_w, &render_h);
        SDL_GetWindowSize(graphics->game_window, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(graphics->game_renderer, scale_x, scale_y);
        graphics->font_scale = scale_y;
    }
    return graphics;
}

void DestroyGraphicsDevice(GraphicsDevice* graphics)
{
    SDL_DestroyRenderer(graphics->game_renderer);
    SDL_DestroyWindow(graphics->game_window);
    free(graphics);
}

