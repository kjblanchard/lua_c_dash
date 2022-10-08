/**
 * @file player_controller.h
 * @brief A controller for a player - Keyboard or joystick user.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-08
 */
#pragma once
#include <SDL2/SDL_scancode.h>
#include "controller_buttons.h"
/**
 * @brief The keybinds for a players keyboard for each of the buttons.
 */
typedef struct KeyboardKeybinds
{
    SDL_Scancode UpButton;
    SDL_Scancode RightButton;
    SDL_Scancode DownButton;
    SDL_Scancode LeftButton;
    SDL_Scancode YButton;
    SDL_Scancode BButton;
    SDL_Scancode AButton;
    SDL_Scancode XButton;
    SDL_Scancode StartButton;
    SDL_Scancode SelectButton;
} KeyboardKeybinds;
/**
 * @brief A players controller.  Contains the player number and his keybinds so that we can check for presses.
 */
typedef struct PlayerController
{
    unsigned char player_number;
    KeyboardKeybinds* keyboard_controls;

} PlayerController;
/**
 * @brief Creates a player controller with default keybinds
 *
 * @param player_number The current players number.
 *
 * @return a new player controller
 */
PlayerController* CreatePlayerController(unsigned char player_number);
/**
 * @brief Destroys a player controller
 *
 * @param controller The controller to destroy.
 */
void DestroyPlayerController(PlayerController* controller);
/**
 * @brief Checks to see if a player controller button is pressed, used by the parent controller.
 *
 * @param player_controller The player controller to check
 * @param button The button to check
 *
 * @return 1 if true, 0 if not.
 */
int IsPlayerControllerButtonPressed(PlayerController* player_controller, ControllerButtons button);
/**
 * @brief Checks to see if a player controllers button is just released, used by the parent controller
 *
 * @param player_controller The player controller to check
 * @param button The button to check
 *
 * @return 1 if true, 0 if not.
 */
int IsPlayerControllerButtonReleased(PlayerController* player_controller, ControllerButtons button);
/**
 * @brief Checks to see if a player controllers button is being held, used by the parent controller
 *
 * @param player_controller The player controller to check
 * @param button The button to check
 *
 * @return 1 if true, 0 if not
 */
int IsPlayerControllerButtonHeld(PlayerController* player_controller, ControllerButtons button);
