#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include "gameobject_lua.h"
#include "../objects/gameobject.h"
#include "../debug/debug.h"

static int LuaCreateGameObject(struct lua_State* state)
{
    float x = luaL_checknumber(state, 1);
    float y = luaL_checknumber(state, 2);
    luaL_checktype(state, 3, LUA_TTABLE);
    int reference = luaL_ref(state,LUA_REGISTRYINDEX);
    Vector2 location = CreateVector2(x, y);
    LogWarn("X is %f and y is %f", location.x, location.y);
    lua_pushlightuserdata(state, CreateGameObject(location, reference));
    //THe number of results is what we return
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

    //Get from the file we read the Player base.
    lua_getglobal(state, "NewPlayer");
    //Gets the value from a table.  So, we got the Player table, now we need to get the new function value and puts it on the stack
    //Get the player global
    const char* name = "HelloChump";
    lua_pushstring(state,name);
    //Call Player.New
    if(lua_pcall(state,1,1,0) != LUA_OK)
        //Get the string from the stack
        LogWarn("Error: %s", lua_tostring(state, -1));
    //This pushes the integer we need to reference onto the stack
    GameObject* gameobject_ref = (GameObject*)lua_topointer(state, -1);
    LogWarn("The gameobject has a locationx of  %f and y is %f, and id is %d", gameobject_ref->location.x, gameobject_ref->location.y, gameobject_ref->id);
    lua_pop(state, 1);

    lua_pushnumber(state, gameobject_ref->lua_ref);
    lua_gettable(state, LUA_REGISTRYINDEX);
    //Lua table /userdata of the player is on stack now
    lua_getglobal(state, "Player");
    //Gets the value from a table.  So, we got the Player table, now we need to get the new function value and puts it on the stack
    lua_getfield(state, -1, "Start");
    //Now the Player.Start function is on the stack as the function to call
    lua_pushnumber(state, gameobject_ref->lua_ref);
    lua_gettable(state, LUA_REGISTRYINDEX);
    //Use the first parameter as the value we stored in the registry of the lua class which is the self parameter in the lua function.
    if(lua_pcall(state,1,0,0) != LUA_OK)
        //Get the string from the stack
        LogWarn("Error2: %s", lua_tostring(state, -1));

    return 1;

}



