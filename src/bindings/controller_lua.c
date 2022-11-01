#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include "controller_lua.h"
#include "../input/controller.h"
#include "../debug/debug.h"

static Controller *CheckController(lua_State *state);
static int LUA_CONTROLLER_AUX_TABLE_REF = 0;

static Controller *CheckController(lua_State *state)
{
    Controller **controller = (Controller **)luaL_checkudata(state, 1, "Lua.Controller");
    luaL_argcheck(state, controller != NULL, 1, "Controller expected");
    return *controller;
}

static int LuaPumpControllerEvent(lua_State *state)
{
    Controller *controller = CheckController(state);
    KeyboardEvent *event = PumpKeyboardEventController(controller);
    if(!event)
    {
        lua_pushnil(state);
        lua_pushnil(state);
        return 2;
    }
    lua_pushinteger(state, event->button);
    lua_pushinteger(state, event->state);
    return 2;
}

static int LuaCreateController(lua_State *state)
{
    Controller **controller_ptr_ptr = (Controller **)lua_newuserdata(state, sizeof(Controller *));
    luaL_getmetatable(state, "Lua.Controller");
    lua_setmetatable(state, -2);
    Controller *controller = CreateController(ControllerType_Player);
    *controller_ptr_ptr = controller;
    return 1;
}

static int LuaDestroyController(lua_State *state)
{
    Controller *controller = CheckController(state);
    DestroyController(controller);
    return 0;
}

static int InitializeLuaController(lua_State *state)
{
    // lua stack: aux table, prv table
    luaL_newmetatable(state, "Lua.Controller");
    static const struct luaL_Reg gameobject_methods[] = {
        {"CreateController", LuaCreateController},
        {"HandleInput", LuaPumpControllerEvent},
        {"DestroyController", LuaDestroyController},
        {NULL, NULL}};
    lua_pushvalue(state, 1);
    LUA_CONTROLLER_AUX_TABLE_REF = luaL_ref(state, LUA_REGISTRYINDEX);
    lua_pushvalue(state, -1);
    luaL_setfuncs(state, gameobject_methods, 0);
    return 1;
}

static int luaopen_Controller(struct lua_State *state)
{
    lua_pushcfunction(state, InitializeLuaController);
    return 1;
}


void RegisterControllerToLuaLibrary(struct lua_State *state)
{
    luaL_requiref(state, "Controller", luaopen_Controller, 0);
}
