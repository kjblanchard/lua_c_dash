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
#include "input.h"

#define MAX_EVENTS 20

union SDL_Event;
struct KeyboardEvent;

/**
 * @brief The keybinds for a players keyboard for each of the buttons.
 */
typedef struct ControllerKeybinds
{
    int UpButton;
    int RightButton;
    int DownButton;
    int LeftButton;
    int YButton;
    int BButton;
    int AButton;
    int XButton;
    int StartButton;
    int SelectButton;
} ControllerKeybinds;

/**
 * @brief A players controller.  Contains the player number and his keybinds so that we can check for presses.
 */
typedef struct PlayerController
{
    unsigned char player_number;
    KeyboardEvent current_events[MAX_EVENTS];
    int current_number_of_events;
    int current_number_buttons_down;
    int current_buttons_down[ControllerButtons_Max];
    int keyboard_lookup_array[ControllerButtons_Max];
    int joystick_lookup_array[ControllerButtons_Max];

} PlayerController;
/**
 * @brief Creates a player controller with default keybinds
 *
 * @param player_number The current players number.
 *
 * @return a new player controller
 */
PlayerController *CreatePlayerController(unsigned char player_number);
/**
 * @brief Destroys a player controller
 *
 * @param controller The controller to destroy.
 */
void DestroyPlayerController(PlayerController *controller);
void PlayerControllerInputReceive(PlayerController *controller, union SDL_Event *event);
void PlayerControllerJoystickInputReceive(PlayerController *controller, union SDL_Event *event);
struct KeyboardEvent *PopKeyboardEvent(PlayerController *controller);
