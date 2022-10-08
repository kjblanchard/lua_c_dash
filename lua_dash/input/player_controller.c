#include <stdlib.h>
#include "player_controller.h"
#include "input.h"

static KeyboardKeybinds* GetDefaultKeyboardKeybinds()
{
    KeyboardKeybinds* bindings = malloc(sizeof(*bindings));
    bindings->UpButton = SDL_SCANCODE_W;
    bindings->RightButton = SDL_SCANCODE_D;
    bindings->DownButton = SDL_SCANCODE_S;
    bindings->LeftButton = SDL_SCANCODE_A;
    bindings->YButton = SDL_SCANCODE_1;
    bindings->BButton = SDL_SCANCODE_X;
    bindings->AButton = SDL_SCANCODE_SPACE;
    bindings->XButton = SDL_SCANCODE_2;
    bindings->StartButton = SDL_SCANCODE_RETURN;
    bindings->SelectButton = SDL_SCANCODE_TAB;
    return bindings;
}

PlayerController* CreatePlayerController(unsigned char player_number)
{
    PlayerController* player_controller = malloc(sizeof(*player_controller));
    player_controller->keyboard_controls = GetDefaultKeyboardKeybinds();
    player_controller->player_number = player_number;
    return player_controller;
}

void DestroyPlayerController(PlayerController* controller)
{
    free(controller->keyboard_controls);
    free(controller);
    controller = NULL;
}

int IsPlayerControllerButtonPressed(PlayerController* player_controller, ControllerButtons button)
{
    switch (button)
    {
        case ControllerButton_Up:
            return IsKeyJustPressed(player_controller->keyboard_controls->UpButton);
        case ControllerButton_Right:
            return IsKeyJustPressed(player_controller->keyboard_controls->RightButton);
        case ControllerButton_Down:
            return IsKeyJustPressed(player_controller->keyboard_controls->DownButton);
        case ControllerButton_Left:
            return IsKeyJustPressed(player_controller->keyboard_controls->LeftButton);
        case ControllerButton_Y:
            return IsKeyJustPressed(player_controller->keyboard_controls->YButton);
        case ControllerButton_B:
            return IsKeyJustPressed(player_controller->keyboard_controls->BButton);
        case ControllerButton_A:
            return IsKeyJustPressed(player_controller->keyboard_controls->AButton);
        case ControllerButton_X:
            return IsKeyJustPressed(player_controller->keyboard_controls->XButton);
        case ControllerButton_Start:
            return IsKeyJustPressed(player_controller->keyboard_controls->StartButton);
        case ControllerButton_Select:
            return IsKeyJustPressed(player_controller->keyboard_controls->SelectButton);
        default:
            return 0;
    }
}

int IsPlayerControllerButtonReleased(PlayerController* player_controller, ControllerButtons button)
{
    switch (button)
    {
        case ControllerButton_Up:
            return IsKeyJustReleased(player_controller->keyboard_controls->UpButton);
        case ControllerButton_Right:
            return IsKeyJustReleased(player_controller->keyboard_controls->RightButton);
        case ControllerButton_Down:
            return IsKeyJustReleased(player_controller->keyboard_controls->DownButton);
        case ControllerButton_Left:
            return IsKeyJustReleased(player_controller->keyboard_controls->LeftButton);
        case ControllerButton_Y:
            return IsKeyJustReleased(player_controller->keyboard_controls->YButton);
        case ControllerButton_B:
            return IsKeyJustReleased(player_controller->keyboard_controls->BButton);
        case ControllerButton_A:
            return IsKeyJustReleased(player_controller->keyboard_controls->AButton);
        case ControllerButton_X:
            return IsKeyJustReleased(player_controller->keyboard_controls->XButton);
        case ControllerButton_Start:
            return IsKeyJustReleased(player_controller->keyboard_controls->StartButton);
        case ControllerButton_Select:
            return IsKeyJustReleased(player_controller->keyboard_controls->SelectButton);
        default:
            return 0;
    }
}

int IsPlayerControllerButtonHeld(PlayerController* player_controller, ControllerButtons button)
{
    switch (button)
    {
        case ControllerButton_Up:
            return IsKeyHeldDown(player_controller->keyboard_controls->UpButton);
        case ControllerButton_Right:
            return IsKeyHeldDown(player_controller->keyboard_controls->RightButton);
        case ControllerButton_Down:
            return IsKeyHeldDown(player_controller->keyboard_controls->DownButton);
        case ControllerButton_Left:
            return IsKeyHeldDown(player_controller->keyboard_controls->LeftButton);
        case ControllerButton_Y:
            return IsKeyHeldDown(player_controller->keyboard_controls->YButton);
        case ControllerButton_B:
            return IsKeyHeldDown(player_controller->keyboard_controls->BButton);
        case ControllerButton_A:
            return IsKeyHeldDown(player_controller->keyboard_controls->AButton);
        case ControllerButton_X:
            return IsKeyHeldDown(player_controller->keyboard_controls->XButton);
        case ControllerButton_Start:
            return IsKeyHeldDown(player_controller->keyboard_controls->StartButton);
        case ControllerButton_Select:
            return IsKeyHeldDown(player_controller->keyboard_controls->SelectButton);
        default:
            return 0;
    }
}
