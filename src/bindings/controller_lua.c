#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
static int hello = 0;
#pragma clang diagnostic pop
//static int LuaCheckIfButtonPressed(lua_State* state)
//{
//    GameObject* gameobject = CheckGameObject(state);
//    int button = luaL_checknumber(state, 2);
//    lua_pushboolean(state, IsControllerButtonPressed(gameobject->controller, button));
//    return 1;
//}
//static int LuaCheckIfButtonHeld(lua_State* state)
//{
//    GameObject* gameobject = CheckGameObject(state);
//    int button = luaL_checknumber(state, 2);
//    lua_pushboolean(state, IsControllerButtonHeld(gameobject->controller, button));
//    return 1;
//}
//static int LuaCheckIfButtonReleased(lua_State* state)
//{
//    GameObject* gameobject = CheckGameObject(state);
//    int button = luaL_checknumber(state, 2);
//    lua_pushboolean(state, IsControllerButtonReleased(gameobject->controller, button));
//    return 1;
//}
