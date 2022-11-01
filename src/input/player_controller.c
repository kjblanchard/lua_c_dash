#include <stddef.h>
#include <stdlib.h>
#include "SDL2/SDL_events.h"
#include "../debug/debug.h"
#include "SDL2/SDL_scancode.h"
#include "controller.h"
#include "controller_buttons.h"
#include "player_controller.h"
#include "input.h"



static KeyboardKeybinds *GetDefaultKeyboardKeybinds()
{
    KeyboardKeybinds *bindings = malloc(sizeof(*bindings));
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

PlayerController *CreatePlayerController(unsigned char player_number)
{
    PlayerController *player_controller = calloc(1, sizeof(*player_controller));
    //Use calloc here to initialize to 0
    player_controller->keyboard_controls = GetDefaultKeyboardKeybinds();
    player_controller->player_number = player_number;
    AddPlayerControllerToInput(player_controller);
    return player_controller;
}

void DestroyPlayerController(PlayerController *controller)
{
    free(controller->keyboard_controls);
    RemovePlayerControllerFromInput(controller->player_number);
    free(controller);
    controller = NULL;
}

//TODO this is ugly.
static ControllerButtons GetButtonFromScancode(PlayerController *controller, SDL_Scancode code)
{
    if(controller->keyboard_controls->SelectButton == code)
        return ControllerButton_Select;
    else if(controller->keyboard_controls->StartButton == code)
        return ControllerButton_Start;
    else if(controller->keyboard_controls->UpButton == code)
        return ControllerButton_Up;
    else if(controller->keyboard_controls->RightButton == code)
        return ControllerButton_Right;
    else if(controller->keyboard_controls->DownButton == code)
        return ControllerButton_Down;
    else if(controller->keyboard_controls->LeftButton == code)
        return ControllerButton_Left;
    else if(controller->keyboard_controls->AButton == code)
        return ControllerButton_A;
    else if(controller->keyboard_controls->XButton == code)
        return ControllerButton_X;
    else if(controller->keyboard_controls->YButton == code)
        return ControllerButton_Y;
    else if(controller->keyboard_controls->BButton == code)
        return ControllerButton_B;
    return ControllerButtons_Max;
}

//TODO what even is this
void PlayerControllerInputReceive(PlayerController *controller, SDL_Event *event)
{
    int type = event->type;
    if(type != SDL_KEYDOWN && type != SDL_KEYUP)
    {
        LogWarn("Player Controller got an event type that wasn't keydown or keyup, somehow you borked this up");
        return;
    }
    SDL_Scancode code = event->key.keysym.scancode;
    ControllerButtons button = GetButtonFromScancode(controller, code);
    if(button == ControllerButtons_Max)
        return;
    controller->current_events[controller->current_number_of_events].button = button;
    if(type == SDL_KEYDOWN)
    {
            int pressed = controller->current_buttons_down[button];
            if(pressed)
            {
                controller->current_events[controller->current_number_of_events].state = Key_State_Held;
            }
            else
            {
                controller->current_events[controller->current_number_of_events].state = Key_State_Pressed;
                controller->current_buttons_down[button] = 1;
                ++controller->current_number_buttons_down;
            }
    }
    else if(type == SDL_KEYUP)
    {
        controller->current_events[controller->current_number_of_events].state = Key_State_Released;
        controller->current_buttons_down[button] = 0;
        --controller->current_number_buttons_down;
    }
    ++controller->current_number_of_events;

}

KeyboardEvent *PopKeyboardEvent(PlayerController *controller)
{
    if(!controller->current_number_of_events)
    {
        if(controller->current_number_buttons_down)
        {
            for (size_t i = 0; i < ControllerButtons_Max; ++i) {
                if(controller->current_buttons_down[i])
                {
                    controller->current_events[controller->current_number_of_events].state = Key_State_Held;
                    controller->current_events[controller->current_number_of_events].button = i;
                    ++controller->current_number_of_events;
                }
            }

        }
        return NULL;
    }
    return &controller->current_events[--controller->current_number_of_events];
}


