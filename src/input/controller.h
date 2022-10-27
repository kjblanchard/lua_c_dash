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

struct PlayerController;
struct AiController;
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
/**
 * @brief Checks to see if a Controller button is just pressed on a controller.
 *
 * @param controller The controller to check
 * @param button The button to check for
 *
 * @return 1 if pressed, 0 if not.
 */
int IsControllerButtonPressed(Controller *controller, ControllerButtons button);
/**
 * @brief Checks to see if a Controller button is Pressed or held on a controller
 *
 * @param controller The controller to check
 * @param button The button to check for
 *
 * @return 1 if pressed, 0 if not.
 */
int IsControllerButtonDown(Controller *controller, ControllerButtons button);
/**
 * @brief Checks to see if a controller button is just released on a controller.
 *
 * @param controller The controller to check
 * @param button The button to check
 *
 * @return 1 if pressed, 0 if not.
 */
int IsControllerButtonReleased(Controller *controller, ControllerButtons button);
/**
 * @brief  Checks to see if a button is being held on a controller.
 *
 * @param controller The controller to check
 * @param button The button to check
 *
 * @return 1 if pressed, 0 if not
 */
int IsControllerButtonHeld(Controller *controller, ControllerButtons button);
