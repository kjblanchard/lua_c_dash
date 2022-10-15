#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include "gameobject_lua.h"
#include "../objects/gameobject.h"
#include "../debug/debug.h"

/**
 * @brief Function to create a gameobject, meant to be called from lua
 *
 * @param state The lua state
 *
 * @return The amount of return values lua should get.
 */
static int LuaCreateGameObject(struct lua_State* state)
{
    int stack_size = lua_gettop(state);
    if(stack_size != 3)
    {
        LogError("GameObject.New call from lua requires 3 arguments: LuaObject,x,y; %d arguments given", stack_size);
        return 0;
    }
    luaL_checktype(state, 1, LUA_TTABLE);
    float x = luaL_checknumber(state, 2);
    float y = luaL_checknumber(state, 3);
    lua_pop(state, 2);
    int reference = luaL_ref(state,LUA_REGISTRYINDEX);
    Vector2 location = CreateVector2(x, y);
    LogWarn("X is %f and y is %f", location.x, location.y);
    lua_pushlightuserdata(state, CreateGameObject(location, reference));
    return 1;
}
/**
 * @brief luaopen_require Is a function called when lua attempts to do a require('require') call, so we return a table with function values.
 *
 * @param state The lua state where we put this
 *
 * @return the amount of return values lua should get.
 */
static int luaopen_GameObject(struct lua_State* state)
{
    //Create an array of lua function names, to references to their functions.  Null/null ends
    static const struct luaL_Reg gameobject_functions [] = {
      {"New", LuaCreateGameObject},
      {NULL, NULL}
    };
    //Create a new table on the stack
    lua_newtable(state);
    //Add functions to it
    luaL_setfuncs(state, gameobject_functions, 0);
    //Tell lua we are returning one thing (the table with the functions)
    return 1;

}
/**
 * @brief Loads the gameobject library into lua, so that we can call it with require.
 *
 * @param state The lua state to push the item.
 *
 * @return 
 */
static int RegisterGameObjectToLuaLibrary(struct lua_State* state)
{
    luaL_requiref(state, "GameObject", luaopen_GameObject, 0);
    return 1;
}

int RunLuaScript(struct lua_State* state)
{
    RegisterGameObjectToLuaLibrary(state);
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



