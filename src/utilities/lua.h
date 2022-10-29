/**
 * @file lua.h
 * @brief Lua utility functions for this engine
 * @author Kevin Blanchard
 * @version v0.1.0
 * @date 2022-10-29
 */
#pragma once

struct lua_State;
typedef struct lua_State lua_State;

/**
 * @brief Initializes lua so that it can be used throughout the engine
 *
 * @param state The state file that we should initialize to
 * TODO we should probably create the state in here, and return it.
 *
 * @return 1 if successful.
 */
int InitializeLua(lua_State *state);
/**
 * @brief Prints out the entire lua stack and gives you info about it.
 *
 * @param state The lua state to check.
 */
void DumpLuaStack (lua_State *state);
