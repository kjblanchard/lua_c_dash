/**
 * @file gameobject_lua.h
 * @brief Handles subscribing and holds the functions for lua to call in reference to gameobjects.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-11
 */
#pragma once

struct lua_State;


int RunLuaScript(struct lua_State* state);
