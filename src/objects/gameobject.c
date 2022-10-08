#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "gameobject.h"
#include "../debug/debug.h"
#include "../input/controller.h"

static unsigned int current_id = 0;
//static const char* main_character_script_name = "./scripts/player.lua";
//static const char* test_script_name = "./scripts/temp.lua";

GameObject* CreateGameObject(Vector2 location, lua_State* state)
{
    GameObject* gameobject = malloc(sizeof(*gameobject));
    gameobject->id = current_id++;
    gameobject->location = location;
    gameobject->controller = CreateController(ControllerType_Player);
    //This is required for print.
    luaL_openlibs(state);
    if(luaL_loadfile(state, "./scripts/temp.lua") || lua_pcall(state, 0, 0, 0))
        LogError("Error, cannot load script file");
    lua_getglobal(state, "Start");
    //Calling global in state, with 0 arguments, and 0 result; last arg is a position on stack where error handling func is, this func returns the error state.
    if(lua_pcall(state, 0, 0, 0) != LUA_OK)
        LogError("Error %s", lua_tostring(state,-1));
    return gameobject;
}

void DestroyGameObject(GameObject* gameobject)
{
    DestroyController(gameobject->controller);
    free(gameobject);
    gameobject = NULL;
}
