#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include "gameobject_lua.h"
#include "../objects/gameobject.h"
#include "../debug/debug.h"

GameObject *gameobject_array[10];

static int LuaGetX(lua_State *state);
static int LuaSetX(lua_State *state);
static int LuaGetY(lua_State *state);
static int LuaSetY(lua_State *state);

static int LUA_GAMEOBJECT_AUX_TABLE_REF = 0;

static GameObject *CheckGameObject(lua_State *state);

static int LuaGetX(lua_State *state)
{
    GameObject *gameobject = CheckGameObject(state);
    lua_pushnumber(state, gameobject->location.x);
    return 1;
}
static int LuaSetX(lua_State *state)
{
    GameObject *gameobject = CheckGameObject(state);
    int x = luaL_checknumber(state, 2);
    gameobject->location.x = x;
    return 0;
}
static int LuaGetY(lua_State *state)
{
    GameObject *gameobject = CheckGameObject(state);
    lua_pushnumber(state, gameobject->location.y);
    return 1;
}
static int LuaSetY(lua_State *state)
{
    GameObject *gameobject = CheckGameObject(state);
    int y = luaL_checknumber(state, 2);
    gameobject->location.y = y;
    return 0;
}
static int LuaSetY(lua_State *state);

/**
 * @brief Function to create a gameobject, meant to be called from lua
 *
 * @param state The lua state
 *
 * @return The amount of return values lua should get.
 */
static int LuaCreateGameObject(struct lua_State *state)
{
    // Stack is x y

    float x = luaL_checknumber(state, 1);
    float y = luaL_checknumber(state, 2);
    Vector2 location = CreateVector2(x, y);
    LogWarn("X is %f and y is %f", location.x, location.y);
    // Pack gameobject in userdata, which also puts it on the stack.
    GameObject **gameobject = (GameObject **)lua_newuserdata(state, sizeof(GameObject *));
    luaL_getmetatable(state, "Lua.GameObject");
    lua_setmetatable(state, -2);
    GameObject *go = CreateGameObject(location);
    *gameobject = go;
    // x/y/gameobj
    // LogWarn("ID of the gameobject not on the stack is %d and x is %f and y is %f ", gameobject->id,gameobject->location.x,gameobject->location.y);
    // Return userdata and address of gameobject
    lua_pushlightuserdata(state, *gameobject);
    return 2;
}

static int LuaGetGameObjectId(lua_State *state)
{
    GameObject *gameobject = CheckGameObject(state);
    lua_pushnumber(state, gameobject->id);
    return 1;
}

static int LuaGetGameObjectLocation(lua_State *state)
{
    GameObject *gameobject = CheckGameObject(state);
    lua_pushnumber(state, gameobject->location.x);
    lua_pushnumber(state, gameobject->location.y);
    return 2;
}

static int LuaSetGameObjectLocation(lua_State *state)
{
    GameObject *gameobject = CheckGameObject(state);
    int x = luaL_checknumber(state, 2);
    int y = luaL_checknumber(state, 3);
    Vector2 new_vector2 = CreateVector2(x, y);
    gameobject->location = new_vector2;
    return 0;
}
/**
 * @brief Checks to see if we passed in a proper gameobject userdata, and then returns a pointer to the gameobject.  Used in all gameobject functions lua calls
 *
 * @param state The lua State
 *
 * @return Pointer to a gameobject that the functions will use
 */
static GameObject *CheckGameObject(lua_State *state)
{

    GameObject **gameobject = (GameObject **)luaL_checkudata(state, 1, "Lua.GameObject");
    luaL_argcheck(state, gameobject != NULL, 1, "`gameobject' expected");
    // return (GameObject *) gameobject;
    return *gameobject;
}

static int initialize(lua_State *state)
{
    luaL_newmetatable(state, "Lua.GameObject");
    // lua stack: aux table, prv table

    // Create our functions we are linking
    static const struct luaL_Reg gameobject_methods[] = {
        {"Create", LuaCreateGameObject},
        {"Id", LuaGetGameObjectId},
        {"Location", LuaGetGameObjectLocation},
        {"SetLocation", LuaSetGameObjectLocation},
        {"X", LuaGetX},
        {"Y", LuaGetY},
        {"SetX", LuaSetX},
        {"SetY", LuaSetY},
        {NULL, NULL}};
    // Set the "aux" table as environment
    lua_pushvalue(state, 1);
    // Store the GameobjectAux table as a reference in the Lua ref table so that we can reference it later
    LUA_GAMEOBJECT_AUX_TABLE_REF = luaL_ref(state, LUA_REGISTRYINDEX);
    LogWarn("Table ref is %d, and 0", LUA_GAMEOBJECT_AUX_TABLE_REF);
    // It is popped off now?
    // Push the table on top of the stack "prv table"
    lua_pushvalue(state, -1);
    luaL_setfuncs(state, gameobject_methods, 0);
    // Tell lua we are returning one thing (the table with the functions)
    return 1;
}
/**
 * @brief luaopen_require Is a function called when lua attempts to do a require('require') call, so we return a table with a initialize function to setup our lua object
 *
 * @param state The lua state where we put this
 *
 * @return The amount of return values, in this case we are pushing one function back.
 */
static int luaopen_GameObject(struct lua_State *state)
{
    lua_pushcfunction(state, initialize);
    return 1;
}
/**
 * @brief Loads the gameobject library into lua, so that we can call it with require.
 *
 * @param state The lua state to push the item.
 *
 * @return
 */
static int RegisterGameObjectToLuaLibrary(struct lua_State *state)
{

    luaL_requiref(state, "GameObject", luaopen_GameObject, 0);
    return 1;
}

int RunLuaScript(struct lua_State *state)
{
    RegisterGameObjectToLuaLibrary(state);
    // Opens all the libraries, this is needed if we need to print something in lua.
    // Load the temp script file, and make sure it works.
    if (luaL_loadfile(state, "./scripts/player.lua") || lua_pcall(state, 0, 0, 0))
        LogError("Error: %s", lua_tostring(state, -1));

    // Create a player which has a gameobject
    lua_getglobal(state, "NewPlayer");
    // Gets the value from a table.  So, we got the Player table, now we need to get the new function value and puts it on the stack
    // Get the player global
    const char *name = "HelloChump";
    lua_pushstring(state, name);
    // Call Player.New
    if (lua_pcall(state, 1, 1, 0) != LUA_OK)
    {
        LogWarn("Error2k: %s", lua_tostring(state, -1));
    }
    //        //Get the string from the stack
    GameObject *created_object = (GameObject *)lua_topointer(state, -1);
    gameobject_array[0] = created_object;
    LogWarn("Gameobject id is %d ", created_object->id);
    return 1;
}

void UpdateAllGameObjects(struct lua_State *state)
{
    GameObject *gameobject = gameobject_array[0];
    lua_rawgeti(state, LUA_REGISTRYINDEX, LUA_GAMEOBJECT_AUX_TABLE_REF);
    lua_getfield(state, -1, "start_gameobject");
    lua_pushlightuserdata(state, gameobject);
    if (lua_pcall(state, 1, 1, 0) != LUA_OK)
        LogWarn("Error3k: %s", lua_tostring(state, -1));

    return;
}
