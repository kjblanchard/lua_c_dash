/**
 * @file controller_lua.h
 * @brief Handles the lua bindings for controller functionality.  Probably will move
 * To a controller component in the future.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-24
 */

struct lua_State;

/**
 * @brief Registers the function so that we can open a lua_controller with Require
 * Should be done with require("Controller")
 *
 * @param state The lua state to load this into
 *
 * @return 
 */
void RegisterControllerToLuaLibrary(struct lua_State* state);

