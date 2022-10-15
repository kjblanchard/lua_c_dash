/**
 * @file gameobject.h
 * @brief Gameobjects.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-07
 */
#pragma once
#include "../base/vector2.h"
struct Controller;


/**
 * @brief The base object of everything in the engine.
 */
typedef struct GameObject
{
    /**
     * @brief The unique id of the gameobject.
     */
    unsigned int id;
    /**
     * @brief The current location of the gameobject.
     */
    Vector2 location;

    int lua_object_reference;

    //Temporary values for testing
    struct Controller* controller;

} GameObject;
/**
 * @brief Creates a new gameobject, and assigns it's proper values
 *
 * @param location the location of this gameobject
 * @param lua_ref the lua reference id that we should use when we need to reference or call functions on this lua object.
 *
 * @return An initialized gameobject.
 */
GameObject* CreateGameObject(Vector2 location, int lua_object_reference);

void PrintGameObjectId(GameObject* gameobject);
/**
 * @brief Cleans up a gameobject for you.
 *
 * @param gameobject The gameobject to destroy.
 */
void DestroyGameObject(GameObject* gameobject);
