/**
 * @file gameobject_lua.h
 * @brief Handles subscribing and holds the functions for lua to call in reference to gameobjects.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-11
 */
#pragma once

struct lua_State;

//Vector2 location is something that we need.
int LuaCreateGameObject(struct lua_State* state);
int LuaGetGameObjectPosition(struct lua_State* state);
int LuaSetGameObjectPosition(struct lua_State* state);

int RegisterAllGameobjectFunctions(struct lua_State* state);
int RunLuaScript(struct lua_State* state);
