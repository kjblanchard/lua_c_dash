#include <stdlib.h>
#include <math.h>
#include "gameobject.h"
#include "../debug/debug.h"
#include "../input/controller.h"

static unsigned int current_id = 0;
//

GameObject* CreateGameObject(Vector2 location, int lua_ref)
{
    GameObject* gameobject = malloc(sizeof(*gameobject));
    gameobject->id = current_id++;
    gameobject->location = location;
    gameobject->lua_ref = lua_ref;
    gameobject->controller = CreateController(ControllerType_Player);
    LogInfo("Just created a gameobject with id %f, locationx %f and locationy %f", gameobject->id, gameobject->location.x, gameobject->location.y);
    return gameobject;
}

void PrintGameObjectId(GameObject* gameobject)
{
    if(!gameobject)
    {
        LogWarn("This gameobject doesn't exist");
        return;
    }
    LogInfo("The gameobject you passed in has the id of %d", gameobject->id);
}

void DestroyGameObject(GameObject* gameobject)
{
    DestroyController(gameobject->controller);
    free(gameobject);
    //TODO release the lua integer ref so it can be reused again.
    gameobject = NULL;
}
