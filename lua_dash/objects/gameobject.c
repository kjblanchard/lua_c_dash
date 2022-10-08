#include <stdlib.h>
#include "gameobject.h"
#include "../input/player_controller.h"

static unsigned int current_id = 0;

GameObject* CreateGameObject(Vector2 location)
{
    GameObject* gameobject = malloc(sizeof(*gameobject));
    gameobject->id = current_id++;
    gameobject->location = location;
    gameobject->controller = CreatePlayerController(0);
    return gameobject;
}

void DestroyGameObject(GameObject* gameobject)
{
    free(gameobject);
    gameobject = NULL;
}
