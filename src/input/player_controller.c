#include <stddef.h>
#include <stdlib.h>
#include "SDL2/SDL_events.h"
#include "../debug/debug.h"
#include "SDL2/SDL_scancode.h"
#include "controller.h"
#include "controller_buttons.h"
#include "player_controller.h"
#include "input.h"


static void SetKeyboardLookupArray(PlayerController *controller, ControllerKeybinds *keyboard_bindings);
static void SetJoystickLookupArray(PlayerController *controller, ControllerKeybinds *keyboard_bindings);

static ControllerKeybinds *GetDefaultKeyboardKeybinds()
{
    ControllerKeybinds *bindings = malloc(sizeof(*bindings));
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

static ControllerKeybinds *GetDefaultJoystickKeybinds()
{
    ControllerKeybinds *bindings = malloc(sizeof(*bindings));
    bindings->UpButton = 11;
    bindings->RightButton = 14;
    bindings->DownButton = 12;
    bindings->LeftButton = 13;
    bindings->YButton = 2;
    bindings->BButton = 0;
    bindings->AButton = 1;
    bindings->XButton = 3;
    bindings->StartButton = 6;
    bindings->SelectButton = 4;
    return bindings;
}


PlayerController *CreatePlayerController(unsigned char player_number)
{
    //Use calloc here to initialize to 0
    PlayerController *player_controller = calloc(1, sizeof(*player_controller));
    SetKeyboardLookupArray(player_controller, GetDefaultKeyboardKeybinds());
    SetJoystickLookupArray(player_controller, GetDefaultJoystickKeybinds());
    player_controller->player_number = player_number;
    AddPlayerControllerToInput(player_controller);
    return player_controller;
}

void DestroyPlayerController(PlayerController *controller)
{
    RemovePlayerControllerFromInput(controller->player_number);
    free(controller);
    controller = NULL;
}

//TODO why is there two??
static void SetKeyboardLookupArray(PlayerController *controller, ControllerKeybinds *keyboard_bindings)
{
    controller->keyboard_lookup_array[ControllerButton_A] = keyboard_bindings->AButton;
    controller->keyboard_lookup_array[ControllerButton_B] = keyboard_bindings->BButton;
    controller->keyboard_lookup_array[ControllerButton_X] = keyboard_bindings->XButton;
    controller->keyboard_lookup_array[ControllerButton_Y] = keyboard_bindings->YButton;
    controller->keyboard_lookup_array[ControllerButton_Up] = keyboard_bindings->UpButton;
    controller->keyboard_lookup_array[ControllerButton_Down] = keyboard_bindings->DownButton;
    controller->keyboard_lookup_array[ControllerButton_Left] = keyboard_bindings->LeftButton;
    controller->keyboard_lookup_array[ControllerButton_Right] = keyboard_bindings->RightButton;
    free(keyboard_bindings);
}

static void SetJoystickLookupArray(PlayerController *controller, ControllerKeybinds *keyboard_bindings)
{
    controller->joystick_lookup_array[ControllerButton_A] = keyboard_bindings->AButton;
    controller->joystick_lookup_array[ControllerButton_B] = keyboard_bindings->BButton;
    controller->joystick_lookup_array[ControllerButton_X] = keyboard_bindings->XButton;
    controller->joystick_lookup_array[ControllerButton_Y] = keyboard_bindings->YButton;
    controller->joystick_lookup_array[ControllerButton_Up] = keyboard_bindings->UpButton;
    controller->joystick_lookup_array[ControllerButton_Down] = keyboard_bindings->DownButton;
    controller->joystick_lookup_array[ControllerButton_Left] = keyboard_bindings->LeftButton;
    controller->joystick_lookup_array[ControllerButton_Right] = keyboard_bindings->RightButton;
    //TODO why are we freeing here???
    free(keyboard_bindings);
}
static ControllerButtons GetButtonFromScancode(PlayerController *controller, int code)
{
    for (size_t i=0; i<ControllerButtons_Max; ++i)
    {
        LogInfo("The thing we are checking is %d, and we are looking for %d", i, code);
        if(controller->joystick_lookup_array[i] == code)
            return i;
    }
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

void PlayerControllerJoystickInputReceive(PlayerController *controller, union SDL_Event *event)
{
    int type = event->type;
    if(type != SDL_JOYBUTTONUP && type != SDL_JOYBUTTONDOWN)
    {
        LogWarn("Player Controller got an event type that wasn't buttondown or buttonup, somehow you borked this up");
        return;
    }
    ControllerButtons button = GetButtonFromScancode(controller, event->jbutton.button);
    LogInfo("Button is %d", button);
    if(button == ControllerButtons_Max)
        return;
    controller->current_events[controller->current_number_of_events].button = button;
    if(type == SDL_JOYBUTTONDOWN)
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
    else if(type == SDL_JOYBUTTONUP)
    {
        controller->current_events[controller->current_number_of_events].state = Key_State_Released;
        controller->current_buttons_down[button] = 0;
        --controller->current_number_buttons_down;
    }
    ++controller->current_number_of_events;
    LogInfo("The button pressed was %d", button);
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


