#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <string.h>
#include "../core/world.h"
#include "../core/debug.h"
#include "alhelpers.h"
#include "openal.h"

#define MAX_SFX_FROM_CONFIG 20
#define MAX_BGM_FROM_CONFIG 20
/**
 * @brief Structure to hold a bgm with it's loop points.
 */
typedef struct Sg_Bgm {
    char* bgm_name;
    double loop_begin;
    double loop_end;

} Sg_Bgm;

typedef struct Sg_Sfx {
    char* sfx_name;
    Sg_Loaded_Sfx* loaded_sfx;
} Sg_Sfx;

static Sg_Bgm** bgm_music;
static Sg_Sfx** sfx_sounds;
static int LoadSfxFromLua(lua_State* state);
static int LoadBgmFromLua(lua_State* state);
static void LoadSoundConfig(lua_State* state);
//TODO is there way to get rid of this?
static const char* sfx_prefix = "build/assets/";



/**
 * @brief Loads the sound table from the already loaded config file, and then loads the Bgm and sfx from it.
 *
 * @param state The lua global state that should be loaded.
 */
static void LoadSoundConfig(lua_State* state)
{
    lua_getglobal(state, "Sound");
    if(!lua_istable(state, -1))
    {
        printf("This isn't a sound table");
        exit(2);
    }
    LoadBgmFromLua(state);
    LoadSfxFromLua(state);
    lua_pop(state, 1);

}

static int LoadBgmFromLua(lua_State* state)
{
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
    //-1 is bgm table at this point.
    //Init with a specific size.
    const int bgm_size = MAX_BGM_FROM_CONFIG;
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
        const char* sfx_suffix = lua_tostring(state, -1);
        size_t name_length = strlen(sfx_prefix) + strlen(sfx_suffix);
        char* full_name = malloc(sizeof(char) * name_length);
        strncpy(full_name,sfx_prefix,strlen(sfx_prefix));
        strcat(full_name,sfx_suffix);
        bgm->bgm_name = full_name;
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
    //Pop off the bgm table
    lua_pop(state, 1);
    return 1;

}
static int LoadSfxFromLua(lua_State* state)
{
    lua_pushstring(state, "Sfx");
    lua_gettable(state, -2);
    if(!lua_istable(state, -1))
    {
        printf("This isn't a proper table for sfx");
        exit(2);
    }
    const int sfx_size = MAX_SFX_FROM_CONFIG;
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
        const char* sfx_suffix = lua_tostring(state, -1);
        size_t name_length = strlen(sfx_prefix) + strlen(sfx_suffix);
        char* full_name = malloc(sizeof(char) * name_length);
        strncpy(full_name,sfx_prefix,strlen(sfx_prefix));
        strcat(full_name,sfx_suffix);
        sfx->sfx_name = full_name;
        lua_pop(state, 1);
        ++i;
    }
    sfx_sounds = calloc(i, sizeof(Sg_Sfx*));
    memcpy(bgm_music,sfx_list,sizeof(Sg_Bgm*) * --i);
    free(sfx_list);
    lua_pop(state,1);
    return 1;

}


int InitializeSound()
{
    LoadSoundConfig(GameWorld->global_lua_state_ptr);
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
    CloseAl();

}
