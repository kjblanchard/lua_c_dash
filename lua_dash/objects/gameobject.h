/**
 * @file gameobject.h
 * @brief Gameobjects.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-07
 */
#pragma once
#include "../base/vector2.h"
struct PlayerController;

/**
 * @brief The base object of everything in the engine.
 */
typedef struct GameObject
{
    unsigned int id;
    struct PlayerController* controller;
    Vector2 location;

} GameObject;
/**
 * @brief Creates a new gameobject, and assigns it's proper values
 *
 * @param location the location of this gameobject
 *
 * @return An initialized gameobject.
 */
GameObject* CreateGameObject(Vector2 location);
/**
 * @brief Cleans up a gameobject for you.
 *
 * @param gameobject The gameobject to destroy.
 */
void DestroyGameObject(GameObject* gameobject);
