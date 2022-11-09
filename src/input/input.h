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


/**
 * @brief The maximum allowed joysticks.
 */
union SDL_Event;
struct PlayerController;
struct _SDL_Joystick;


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
void AddJoystick(union SDL_Event *event);
struct _SDL_Joystick* GetJoystick(int joystick_to_get);

