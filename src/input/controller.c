#include <stdlib.h>
#include "../debug/debug.h"
#include "input.h"
#include "controller.h"
#include "player_controller.h"
#include "ai_controller.h"

Controller *CreateController(ControllerTypes controller_type)
{
    Controller *controller = malloc(sizeof(*controller));
    switch (controller_type)
    {
    case ControllerType_Default:
        LogWarn("Trying to create a controller with type Default for some reason");
        DestroyController(controller);
        return NULL;
    case ControllerType_Player:
        controller->controller_type = ControllerType_Player;
        controller->internal_controller.player_controller = CreatePlayerController(0);
        break;
    case ControllerType_Ai:
        LogInfo("Ai controller type is Not implemented");
        DestroyController(controller);
        return NULL;
    }
    return controller;
}

void DestroyController(Controller *controller)
{
    switch (controller->controller_type)
    {
    case ControllerType_Default:
        LogWarn("Trying to destroy a controller with the type Default for some reason");
        break;
    case ControllerType_Player:
        DestroyPlayerController(controller->internal_controller.player_controller);
        break;
    case ControllerType_Ai:
        LogInfo("Controller type AI is not implemented");
        break;
    }
    free(controller);
    controller = NULL;
}

KeyboardEvent *PumpKeyboardEventController(Controller *controller)
{
    switch (controller->controller_type)
    {
    case ControllerType_Default:
        LogWarn("Trying to pump a controller with the type Default for some reason");
        break;
    case ControllerType_Player:
        return PopKeyboardEvent(controller->internal_controller.player_controller);
        break;
    case ControllerType_Ai:
        LogInfo("Controller type AI is not implemented");
        break;
    }
    return NULL;

}
