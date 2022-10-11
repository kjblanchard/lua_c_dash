#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "gameobject_lua.h"
#include "../objects/gameobject.h"
#include "../debug/debug.h"

int LuaCreateGameObject(struct lua_State* state)
{

    LogWarn("Just did the thing boi");

    return 1;

}

int LuaGetGameObjectPosition(struct lua_State* state)
{
    return 1;

}

int LuaSetGameObjectPosition(struct lua_State* state)
{
    return 1;

}

int RegisterAllGameobjectFunctions(struct lua_State* state)
{
    //First we push the c function onto the stack
    lua_pushcfunction(state, LuaCreateGameObject);
    //Then we set a global, with the first thing on the stack with the name.
    lua_setglobal(state,"GameObject");
    return 1;

}

int RunLuaScript(struct lua_State* state)
{
    RegisterAllGameobjectFunctions(state);
    //Opens all the libraries, this is needed if we need to print something in lua.
    luaL_openlibs(state);
    //Load the temp script file, and make sure it works.
    if(luaL_loadfile(state, "./scripts/temp.lua") || lua_pcall(state, 0, 0, 0))
        LogError("Error, cannot load script file");
    //TODO run the script's function?
    lua_getglobal(state, "Start");
    ////Calling global in state, with 0 arguments, and 0 result; last arg is a position on stack where error handling func is, this func returns the error state.
    if(lua_pcall(state, 0, 0, 0) != LUA_OK)
        LogError("Error %s", lua_tostring(state,-1));
    return 1;

}



