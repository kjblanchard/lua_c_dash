#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "controller_lua.h"
#include "../input/controller.h"
#include "../debug/debug.h"

static Controller* CheckController (lua_State *state);
static int LUA_CONTROLLER_AUX_TABLE_REF = 0;

static Controller* CheckController (lua_State *state) {

      Controller** controller = (Controller**)luaL_checkudata(state, 1, "Lua.Controller");
      luaL_argcheck(state, controller != NULL, 1, "Controller expected");
      return *controller;
    }

static int LuaCheckIfButtonPressed(lua_State* state)
{
    Controller* controller = CheckController(state);
    int button = luaL_checknumber(state, 2);
    lua_pushboolean(state, IsControllerButtonPressed(controller, button));
    return 1;
}

static int LuaCheckIfButtonHeld(lua_State* state)
{
    Controller* controller = CheckController(state);
    int button = luaL_checknumber(state, 2);
    lua_pushboolean(state, IsControllerButtonHeld(controller, button));
    return 1;
}

static int LuaCheckIfButtonReleased(lua_State* state)
{
    Controller* controller = CheckController(state);
    int button = luaL_checknumber(state, 2);
    lua_pushboolean(state, IsControllerButtonReleased(controller, button));
    return 1;
}

static int InitializeLuaController(lua_State* state)
{
    //lua stack: aux table, prv table
    luaL_newmetatable(state, "Lua.Controller");
    static const struct luaL_Reg gameobject_methods [] = {
        {"CheckIfButtonPressed", LuaCheckIfButtonPressed},
        {"CheckIfButtonReleased", LuaCheckIfButtonReleased},
        {"CheckIfButtonHeld", LuaCheckIfButtonHeld},
        {NULL, NULL}
    };
    lua_pushvalue(state, 1);
    LUA_CONTROLLER_AUX_TABLE_REF = luaL_ref(state, LUA_REGISTRYINDEX);
    LogWarn("Table ref is %d, and 0", LUA_CONTROLLER_AUX_TABLE_REF);
    lua_pushvalue(state, -1);
    luaL_setfuncs(state, gameobject_methods, 0);
    return 1;
}

static int luaopen_Controller(struct lua_State* state)
{
    lua_pushcfunction(state, InitializeLuaController);
    return 1;
}

void RegisterControllerToLuaLibrary(struct lua_State* state)
{
    luaL_requiref(state, "Controller", luaopen_Controller, 0);
}
