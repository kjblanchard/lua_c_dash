#include <stdlib.h>
#include "../debug/debug.h"
#include "controller.h"
#include "player_controller.h"
#include "ai_controller.h"


Controller* CreateController(ControllerTypes controller_type)
{
    Controller* controller = malloc(sizeof(*controller));
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

void DestroyController(Controller* controller)
{
    switch(controller->controller_type)
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

int IsControllerButtonPressed(Controller* controller, ControllerButtons button)
{
    switch(controller->controller_type)
    {
        case ControllerType_Default:
            LogWarn("Checking if button pressed on a Default typed controller");
            return 0;
        case ControllerType_Player:
            return IsPlayerControllerButtonPressed(controller->internal_controller.player_controller,button);
        case ControllerType_Ai:
            LogWarn("Checking if button pressed on a AI typed controller when it is not implemented");
            return 0;
        default:
            return 0;
    }
}

int IsControllerButtonDown(Controller* controller, ControllerButtons button)
{
    switch(controller->controller_type)
    {
        case ControllerType_Default:
            LogWarn("Checking if button pressed on a Default typed controller");
            return 0;
        case ControllerType_Player:
            return IsPlayerControllerButtonPressed(controller->internal_controller.player_controller,button) || IsPlayerControllerButtonHeld(controller->internal_controller.player_controller, button);
        case ControllerType_Ai:
            LogWarn("Checking if button pressed on a AI typed controller when it is not implemented");
            return 0;
        default:
            return 0;
    }
}

int IsControllerButtonReleased(Controller* controller, ControllerButtons button)
{
    switch(controller->controller_type)
    {
        case ControllerType_Default:
            LogWarn("Checking if button Released on a Default typed controller");
            return 0;
        case ControllerType_Player:
            return IsPlayerControllerButtonReleased(controller->internal_controller.player_controller,button);
        case ControllerType_Ai:
            LogWarn("Checking if button Released on a AI typed controller when it is not implemented");
            return 0;
        default:
            return 0;
    }
}

int IsControllerButtonHeld(Controller* controller, ControllerButtons button)
{
    switch(controller->controller_type)
    {
        case ControllerType_Default:
            LogWarn("Checking if button held on a Default typed controller");
            return 0;
        case ControllerType_Player:
            return IsPlayerControllerButtonHeld(controller->internal_controller.player_controller,button);
        case ControllerType_Ai:
            LogWarn("Checking if button held on a AI typed controller when it is not implemented");
            return 0;
        default:
            return 0;
    }
}
