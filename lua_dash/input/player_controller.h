#pragma once
#include <SDL2/SDL_scancode.h>
#include "controller.h"

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

typedef struct PlayerController
{
    unsigned char player_number;
    Controller* controller;
    KeyboardKeybinds* keyboard_controls;

} PlayerController;

PlayerController* CreatePlayerController(unsigned char player_number);
void DestroyPlayerController(PlayerController* controller);


int IsPlayerControllerButtonPressed(PlayerController* controller, ControllerButtons button);
int IsPlayerControllerButtonReleased(PlayerController* controller, ControllerButtons button);
int IsPlayerControllerButtonHeld(PlayerController* controller, ControllerButtons button);
