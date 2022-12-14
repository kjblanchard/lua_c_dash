#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <string.h>
#include "../core/world.h"
#include "../debug/debug.h"
#include "alhelpers.h"
#include "openal.h"
#include "sound.h"

/**
 * @brief The max amount of sounds we should load.
 */
#define MAX_SFX_FROM_CONFIG 20
/**
 * @brief The max amount of bgm we should load.
 */
#define MAX_BGM_FROM_CONFIG 20
/**
 * @brief Structure to hold a bgm with it's loop points.
 */
typedef struct Bgm
{
    char *bgm_name;
    double loop_begin;
    double loop_end;

} Bgm;

/**
 * @brief Holds a sfx name and the loaded file if it is loaded.
 */
typedef struct Sfx
{
    char *sfx_name;
    Sg_Loaded_Sfx *loaded_sfx;
} Sfx;

/**
 * @brief The number of bgm's that were loaded from the config file
 */
static unsigned char bgm_length = 0;
/**
 * @brief The number of SFX that was loaded from the config file.
 */
static unsigned char sfx_length = 0;
/**
 * @brief A pointer to an array of Sg_Bgms that was loaded from the config file.
 */
static Bgm **bgm_music;
/**
 * @brief A pointer to an array of Sg_Sfx that was loaded from the config file
 */
static Sfx **sfx_sounds;
/**
 * @brief Reads the lua state file, and creates sg_bgms from them that can be used to play music in thhe game.
 *
 * @param state The lua state file that we should use
 *
 * @return 1 on success, 0 on failure.
 */
static int LoadSfxFromLua(lua_State *state);
/**
 * @brief Reads the lua state file, and creates sg_sfx from them that can be used to play sounds in the game.
 *
 * @param state The lua state file that we should use.
 *
 * @return 1 on success, 0 on failure.
 */
static int LoadBgmFromLua(lua_State *state);
/**
 * @brief Loads the BGM and SFX from the lua config file.
 *
 * @param state The lua state that we should use.
 */
static void LoadSoundConfig(lua_State *state);
/**
 * @brief The prefix that we should add to the config file so that we look in the right location.
 */
static const char *sfx_prefix = "assets/";
/**
 * @brief Loads the sound table from the already loaded config file, and then loads the Bgm and sfx from it.
 *
 * @param state The lua global state that should be loaded.
 */
static void LoadSoundConfig(lua_State *state);

int InitializeSound()
{
    LoadSoundConfig(GameWorld->global_lua_state_ptr);
    return InitializeAl();
}

static void LoadSoundConfig(lua_State *state)
{
    int lua_type = lua_getglobal(state, "Sound");
    if (lua_type != LUA_TTABLE)
        // if(!lua_istable(state, -1))
        LogError("This isn't a sound table");
    LoadBgmFromLua(state);
    LoadSfxFromLua(state);
    lua_pop(state, 1);
}

static int LoadBgmFromLua(lua_State *state)
{
    // Push the string bgm to the stack
    lua_pushstring(state, "Bgm");
    // get from the table at location -2 on the stack, using the -1 key, which is Sound[Bgm]
    lua_gettable(state, -2);
    if (!lua_istable(state, -1))
    {
        LogError("This isn't a proper table for bgm");
        return 0;
    }
    // Initialize a temporary holding place for the bgms since we don't know how many there is, with the max amount that we set in the macro.
    Bgm *bgm_list[MAX_BGM_FROM_CONFIG];

    // We need to loop through everything in that table now, lua indexes start at 1, and we don't know how long it is so we will break if index is nil below.
    int i = 1;
    while (1)
    {
        // Push the key onto the stack.  -1 on the stack is the index we need, -2 on the stack is the Table we are iterating through.
        lua_pushinteger(state, i);
        // Try and get from the last table on the stack, remember this uses -1 as the key.
        lua_gettable(state, -2);
        // Check if it is nil, if it is nil, there is no more elements (foreach is ended)
        if (lua_isnil(state, -1))
        {
            // Pop off the table. before we break to clear stack that we had put there.
            lua_pop(state, 1);
            break;
        }
        Bgm *bgm = malloc(sizeof(*bgm));
        lua_getfield(state, -1, "name");
        size_t len;
        const char *sfx_suffix = lua_tolstring(state, -1, &len);
        // We need to add one here, since strlen and len do not include their null terminator, and we need that in our string and we are going to combine things.
        size_t name_length = strlen(sfx_prefix) + len + 1;
        char *full_name = malloc(name_length * sizeof(char));
        snprintf(full_name, name_length, "%s%s", sfx_prefix, sfx_suffix);
        bgm->bgm_name = full_name;
        lua_getfield(state, -2, "loop_start");
        bgm->loop_begin = lua_tonumber(state, -1);
        lua_getfield(state, -3, "loop_end");
        bgm->loop_end = lua_tonumber(state, -1);
        ;
        bgm_list[i - 1] = bgm;
        // Pop off the field value and the table value, and the int value.
        lua_pop(state, 4);
        ++i;
    }
    // Subtract one since lua tables start at 1.
    bgm_length = --i;
    // Create right sized array, and deep copy memory over to it.
    bgm_music = malloc(bgm_length * sizeof(bgm_music));
    memcpy(bgm_music, bgm_list, sizeof(bgm_music) * bgm_length);
    // Pop off the bgm table
    lua_pop(state, 1);
    return 1;
}

static int LoadSfxFromLua(lua_State *state)
{
    lua_pushstring(state, "Sfx");
    lua_gettable(state, -2);
    if (!lua_istable(state, -1))
        printf("This isn't a proper table for sfx");
    Sfx *sfx_list[MAX_SFX_FROM_CONFIG];
    int i = 1;
    while (1)
    {
        lua_pushinteger(state, i);
        lua_gettable(state, -2);
        if (lua_isnil(state, -1))
        {
            lua_pop(state, 1);
            break;
        }
        Sfx *sfx = malloc(sizeof(*sfx));
        lua_getfield(state, -1, "name");
        size_t len;
        const char *sfx_suffix = lua_tolstring(state, -1, &len);
        size_t name_length = strlen(sfx_prefix) + len + 1;
        char *full_name = malloc(name_length * sizeof(char));
        snprintf(full_name, name_length, "%s%s", sfx_prefix, sfx_suffix);
        sfx->sfx_name = full_name;
        sfx->loaded_sfx = NULL;
        sfx_list[i - 1] = sfx;
        lua_pop(state, 1);
        ++i;
    }
    sfx_length = --i;
    sfx_sounds = malloc(sfx_length * sizeof(sfx_sounds));
    memcpy(sfx_sounds, sfx_list, sizeof(sfx_sounds) * sfx_length);
    lua_pop(state, 1);
    return 1;
}

int PlayBgm(int bgm_number, float volume)
{
    if (bgm_number >= bgm_length)
    {
        LogWarn("The bgm number you tried to play doesn't exist.");
        return 0;
    }
    return PlayBgmAl(bgm_music[bgm_number]->bgm_name, &bgm_music[bgm_number]->loop_begin, &bgm_music[bgm_number]->loop_end, volume);
}

int StopBgm(int stop_at_end)
{
    return StopBgmAl();
}
int PauseBgm()
{
    return PauseBgmAl();
}
int UnPauseBgm()
{
    return UnpauseBgmAl();
}

int PlaySfxOneShot(int sfx_number, float volume)
{
    if (sfx_number >= sfx_length)
    {
        LogWarn("The sfx number you tried to play doesn't exist.");
        return 0;
    }
    if (!sfx_sounds[sfx_number]->loaded_sfx)
    {
        sfx_sounds[sfx_number]->loaded_sfx = LoadSfxFileAl(sfx_sounds[sfx_number]->sfx_name);
    }
    PlaySfxAl(sfx_sounds[sfx_number]->loaded_sfx, volume);
    return 1;
}

int LoadSfx(int sfx_number)
{
    if (!sfx_sounds[sfx_number]->loaded_sfx)
    {
        sfx_sounds[sfx_number]->loaded_sfx = LoadSfxFileAl(sfx_sounds[sfx_number]->sfx_name);
    }
    return (sfx_sounds[sfx_number]->loaded_sfx != NULL) ? 1 : 0;
}

int UnloadSfx(int sfx_number)
{
    if (sfx_sounds[sfx_number]->loaded_sfx)
    {
        CloseSfxFileAl(sfx_sounds[sfx_number]->loaded_sfx);
    }
    if (sfx_sounds[sfx_number])
    {
        free(sfx_sounds[sfx_number]->sfx_name);
        sfx_sounds[sfx_number]->sfx_name = NULL;
        free(sfx_sounds[sfx_number]);
        sfx_sounds[sfx_number] = NULL;
    }
    return (sfx_sounds[sfx_number] == NULL) ? 1 : 0;
}

void UpdateSound()
{
    UpdateAl();
}

void CloseSound()
{
    for (int i = 0; i < sfx_length; ++i)
    {
        UnloadSfx(i);
    }
    free(sfx_sounds);
    for (int i = 0; i < bgm_length; ++i)
    {
        free(bgm_music[i]->bgm_name);
        free(bgm_music[i]);
    }
    free(bgm_music);
    CloseAl();
}
