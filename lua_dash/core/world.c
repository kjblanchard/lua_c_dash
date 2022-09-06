#include <SDL2/SDL.h>
#include <lauxlib.h>
#include "world.h"
#include "GraphicsDevice.h"

World* CreateWorld()
{
    if(GameWorld)
        return GameWorld;
    World* world_ptr;
    world_ptr = (World* )malloc(sizeof(World));
    lua_State*L = luaL_newstate();
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
        printf("Error loading sdl, %s",SDL_GetError());
    }
    Mix_Init(MIX_INIT_OGG);
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 0;
        printf("Error loading sdl mixer");
    }
    Mix_Music* loade_muxic = Mix_LoadMUS( "ro.ogg" );
    if(loade_muxic == NULL)
    {
        printf("Music is borked");
        return 0;
    }
    Mix_PlayMusic(loade_muxic, -1);
    GameWorld->playing_music = loade_muxic;
    return 1;
}

void DestroyWorld(World* world)
{
    lua_close(world->global_lua_state_ptr);
    Mix_Quit();
    free(world);
    GameWorld = NULL;
}
