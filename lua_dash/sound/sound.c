#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <string.h>
#include "../core/world.h"
#include "../core/debug.h"
#include "alhelpers.h"
#include "openal.h"

static int LoadSfxFromLua(lua_State* state);
static int LoadBgmFromLua(lua_State* state);
//TODO define this in lua, and create this in C from reading that.
/**
 * @brief Structure to hold a bgm with it's loop points.
 */
typedef struct Sg_Bgm {
    const char* bgm_name;
    double loop_begin;
    double loop_end;

} Sg_Bgm;

typedef struct Sg_Sfx {
    const char* sfx_name;
    Sg_Loaded_Sfx* loaded_sfx;
} Sg_Sfx;

static Sg_Bgm** bgm_music;
static Sg_Sfx** sfx_sounds;

static void load_sound_config(lua_State* state)
{
    //Probably don't need to get this since we are already getting it in graphics.
    //First we load the file chunk with load file, then we use a protectedcall to actually run it.  We print an error if it doesn't work.
    //    if(luaL_loadfile(state, "./build/config.lua") || lua_pcall(state, 0, 0, 0))
    //        printf("Error, cannot run config file");

    lua_getglobal(state, "Sound");
    if(!lua_istable(state, -1))
    {
        printf("This isn't a sound table");
        exit(2);
    }
    //Push the string bgm to the table.
    lua_pushstring(state, "Bgm");
    //get from the table at location -2 on the stack, the -1 key
    //Sound[Bgm]
    lua_gettable(state, -2);
    if(!lua_istable(state, -1))
    {
        printf("This isn't a proper table for bgm");
        exit(2);
    }
    LoadBgmFromLua(state);
    lua_pop(state, 1);
    //LoadSfx
    lua_pushstring(state, "Sfx");
    lua_gettable(state, -2);
    if(!lua_istable(state, -1))
    {
        printf("This isn't a proper table for sfx");
        exit(2);
    }
    LoadSfxFromLua(state);

}

static int LoadBgmFromLua(lua_State* state)
{
    //-1 is bgm table at this point.
    //Init with a specific size.
    const int bgm_size = 20;
    Sg_Bgm** bgm_list = (Sg_Bgm**)calloc(bgm_size, sizeof(bgm_list));

    //We need to loop through everything in that table now.
    //Use ended to note when we should stop.
    int ended = 0;
    int i = 1;
    while(!ended)
    {
        //Push the key onto the stack.
        //-1 on the stack is the index we need, -2 on the stack is the Table we are iteratint through.
        lua_pushinteger(state, i);
        //Try and get from the last table on the stack.
        lua_gettable(state, -2);
        //Check if it is nil, if it is nil, there is no more elements.
        if(lua_isnil(state, -1))
        {
            //Pop off the table.
            lua_pop(state, 1);
            ended = 1;
            break;
        }
        Sg_Bgm* bgm = malloc(sizeof(*bgm));
        lua_getfield(state,-1,"name");
        bgm->bgm_name = lua_tostring(state, -1);
        lua_getfield(state,-2,"loop_start");
        bgm->loop_begin = lua_tonumber(state, -1);
        lua_getfield(state,-3,"loop_end");
        bgm->loop_end= lua_tonumber(state, -1);;
        bgm_list[i-1] = bgm;
        //Pop off the field value and the table value, and the int value.
        lua_pop(state, 4);
        ++i;
    }
    //Copy to right sized array, and destroy temporary one.
    bgm_music = calloc(i, sizeof(Sg_Bgm*));
    memcpy(bgm_music,bgm_list,sizeof(Sg_Bgm*) * --i);
    free(bgm_list);
    //Pop off the table,and sound table from the stack.
    return 1;

}
static int LoadSfxFromLua(lua_State* state)
{
    const int sfx_size = 20;
    Sg_Sfx** sfx_list = (Sg_Sfx**)calloc(sfx_size, sizeof(sfx_list));
    int ended = 0;
    int i = 1;
    while(!ended)
    {
        lua_pushinteger(state, i);
        lua_gettable(state, -2);
        if(lua_isnil(state, -1))
        {
            //Pop off the table.
            lua_pop(state, 1);
            ended = 1;
            break;
        }
        Sg_Sfx* sfx = malloc(sizeof(*sfx));
        lua_getfield(state,-1,"name");
        sfx->sfx_name = lua_tostring(state, -1);
        //sfx_list[i-1] = bgm;
        //Pop off the field value and the table value, and the int value.
        lua_pop(state, 4);
        ++i;
    }
    //Copy to right sized array, and destroy temporary one.
    sfx_sounds = calloc(i, sizeof(Sg_Sfx*));
    memcpy(bgm_music,sfx_list,sizeof(Sg_Bgm*) * --i);
    free(sfx_list);
    //Pop off the table,and sound table from the stack.
    lua_pop(state,2);
    return 1;

}


int InitializeSound()
{
    load_sound_config(GameWorld->global_lua_state_ptr);
    return InitializeAl();
}

int PlayBgm(int bgm_number)
{

    return PlayBgmAl(bgm_music[bgm_number]->bgm_name, &bgm_music[bgm_number]->loop_begin,&bgm_music[bgm_number]->loop_end);

}

int PlaySfxOneShot(int sfx_number)
{
    if (!sfx_sounds[sfx_number]->loaded_sfx)
    {
        sfx_sounds[sfx_number]->loaded_sfx = LoadSfxFileAl(sfx_sounds[sfx_number]->sfx_name);
    }

    PlaySfxAl(sfx_sounds[sfx_number]->loaded_sfx);
    return 1;

}
void UpdateSound()
{
    UpdateAl();
}

void CloseSound()
{
    CloseAL();

}
