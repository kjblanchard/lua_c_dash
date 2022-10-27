#include <lua.h>
#include <lauxlib.h>
#include "lua.h"
#include "../debug/debug.h"

void GetLuaTableSg(lua_State *state, const char *table_name)
{
    lua_getglobal(state, table_name);
    if (!lua_istable(state, -1))
        LogError("This isn't a table");
}
