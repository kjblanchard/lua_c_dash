#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "lua.h"
#include "../debug/debug.h"


static int SetLuaPath(lua_State *state, const char *path)
{
    int value = lua_getglobal(state, "package");
    if (value == LUA_TNIL)
        LogWarn("Borked");
    lua_getfield(state, -1, "path");                    // get field "path" from table at top of stack (-1)
    const char *current_path = lua_tostring(state, -1); // grab path string from top of stack
    size_t full_str_len = strlen(current_path) + strlen(path) + 2;
    char full_str[full_str_len];
    sprintf(full_str, "%s;%s", current_path, path);
    lua_pop(state, 1);               // get rid of the string on the stack we just pushed on line 5
    lua_pushstring(state, full_str); // push the new one
    lua_setfield(state, -2, "path"); // set the field "path" in table at -2 with value at top of stack
    lua_pop(state, 1);               // get rid of package table from top of stack
    return 0;                    // all done!
}

int InitializeLua(lua_State *state)
{
    luaL_openlibs(state);
    SetLuaPath(state, "./scripts/?.lua;");
    return 1;
}

void DumpLuaStack (lua_State *state) {
  int top=lua_gettop(state);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(state,i));
    switch (lua_type(state, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(state,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(state,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(state, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(state,i));
        break;
    }
  }
}
