/**
 * @file controller.h
 * @brief The controller that is used on gameobjects that need to be controlled.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-08
 */
#pragma once
/**
 * @brief All of the buttons that can be pressed in the game.
 */
#include "controller_buttons.h"
#include "player_controller.h"

struct PlayerController;
struct AiController;
struct KeyboardEvent;

/**
 * @brief The types of controllers that we can create.
 */
typedef enum ControllerTypes
{
    ControllerType_Default,
    ControllerType_Player,
    ControllerType_Ai,

} ControllerTypes;
/**
 * @brief Structure that contains the controller to be used.
 */
typedef struct Controller
{
    /**
     * @brief The type of controller this is, this decides all functions used by this.
     */
    ControllerTypes controller_type;
    /**
     * @brief Holds either a player controller or an ai controller, decided on creation.
     */
    union internal_controller
    {
        struct PlayerController *player_controller;
        struct AiController *ai_controller;

    } internal_controller;

} Controller;
/**
 * @brief Creates a controller of a specific type.
 *
 * @param controller_type
 *
 * @return A new controller, or NULL if error.
 */
Controller *CreateController(ControllerTypes controller_type);
/**
 * @brief Destroys a controller
 *
 * @param controller The controller to destroy.
 */
void DestroyController(Controller *controller);
struct KeyboardEvent *PumpKeyboardEventController(Controller *controller);


