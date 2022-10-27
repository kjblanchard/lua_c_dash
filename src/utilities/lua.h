#pragma once

struct lua_State;
typedef struct lua_State lua_State;

void GetLuaTableSg(lua_State *state, const char *table_name);
