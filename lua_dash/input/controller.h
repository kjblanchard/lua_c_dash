#pragma once
/**
 * @brief All of the buttons that can be pressed in the game.
 */
typedef enum ControllerButtons
{
    Default,
    ControllerButton_Up,
    ControllerButton_Right,
    ControllerButton_Down,
    ControllerButton_Left,
    ControllerButton_Y,
    ControllerButton_B,
    ControllerButton_A,
    ControllerButton_X,
    ControllerButton_Start,
    ControllerButton_Select,
} ControllerButtons;
/**
 * @brief Function pointers that return if a button is pressed or held.  Used by player controllers and AI controllers.
 */
typedef struct Controller
{
    int(*IsButtonPressed)(void*,ControllerButtons);
    int(*IsButtonReleased)(void*,ControllerButtons);
    int(*IsButtonHeld)(void*,ControllerButtons);
} Controller;
