#include <stdlib.h>
#include "gameobject.h"
#include "../input/controller.h"

static unsigned int current_id = 0;

GameObject* CreateGameObject(Vector2 location)
{
    GameObject* gameobject = malloc(sizeof(*gameobject));
    gameobject->id = current_id++;
    gameobject->location = location;
    gameobject->controller = CreateController(ControllerType_Player);
    return gameobject;
}

void DestroyGameObject(GameObject* gameobject)
{
    DestroyController(gameobject->controller);
    free(gameobject);
    gameobject = NULL;
}
