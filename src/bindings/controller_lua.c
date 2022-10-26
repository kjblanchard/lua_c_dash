#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
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

static int LuaCreateController(lua_State* state)
{
    Controller** controller_ptr_ptr = (Controller **)lua_newuserdata(state, sizeof(Controller*));
    luaL_getmetatable(state, "Lua.Controller");
    lua_setmetatable(state, -2);
    Controller* controller = CreateController(ControllerType_Player);
    *controller_ptr_ptr = controller;
    return 1;
}

static int InitializeLuaController(lua_State* state)
{
    //lua stack: aux table, prv table
    luaL_newmetatable(state, "Lua.Controller");
    static const struct luaL_Reg gameobject_methods [] = {
        {"CheckIfButtonPressed", LuaCheckIfButtonPressed},
        {"CreateController", LuaCreateController},
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

//TODO this needs to move into it's own file or something.
static int setLuaPath( lua_State* L, const char* path )
{
    int value = lua_getglobal( L, "package" );
    if(value == LUA_TNIL)
        LogWarn("Borked");
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    const char* current_path = lua_tostring( L, -1 ); // grab path string from top of stack
    size_t full_str_len =  strlen(current_path) + strlen(path) +2;
    char full_str[full_str_len];
    sprintf(full_str,"%s;%s",current_path,path);
    lua_pop( L, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( L, full_str ); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}

void RegisterControllerToLuaLibrary(struct lua_State* state)
{
    luaL_openlibs(state);
    setLuaPath(state, "./scripts/?.lua;");
    luaL_requiref(state, "Controller", luaopen_Controller, 0);
}
