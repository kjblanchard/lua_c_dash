/**
 * @file input.h
 * @brief Handles the core input gathering each frame.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-07
 */
#pragma once
#include "SDL2/SDL_events.h"
#include <SDL2/SDL_scancode.h>
#include "controller_buttons.h"

union SDL_Event;
struct PlayerController;


typedef enum KeyStates
{
    Key_State_Pressed,
    Key_State_Held,
    Key_State_Released,

} KeyStates;

typedef struct KeyboardEvent
{
    ControllerButtons button;
    KeyStates state;

} KeyboardEvent;

/**
 * @brief Initializes the Engines input.
 */
void InitializeInput();
void HandleInputEvent(union SDL_Event *event);
void AddPlayerControllerToInput(struct PlayerController *controller);
void RemovePlayerControllerFromInput(int controller_num);
