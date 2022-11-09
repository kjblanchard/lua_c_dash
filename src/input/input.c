#include <stdint.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>
#include <stdlib.h>
#include "../debug/debug.h"
#include "player_controller.h"
#include "input.h"

#define MAX_PLAYERS 4
static int current_player_controllers = 0;
static PlayerController* player_controllers[MAX_PLAYERS];
static int current_joysticks = 0;
static struct _SDL_Joystick* connected_joysticks[MAX_PLAYERS];

void InitializeInput()
{

}

void AddPlayerControllerToInput(PlayerController *controller)
{
    if(current_player_controllers == MAX_PLAYERS)
    {
        LogWarn("Cannot add another controller, already at max players.");
        return;
    }
    player_controllers[current_player_controllers++] = controller;
}

void HandleInputEvent(SDL_Event *event)
{
    for (size_t i = 0; i < current_player_controllers; ++i)
    {
        if(!player_controllers[i])
            return;
        int type = event->type;
        if(type == SDL_KEYDOWN || type == SDL_KEYUP)
        {
            PlayerControllerInputReceive(player_controllers[i], event);
        }
        else if(type == SDL_JOYBUTTONDOWN || type == SDL_JOYBUTTONUP)
        {
            PlayerControllerJoystickInputReceive(player_controllers[i],event);
        }
    }
}

void RemovePlayerControllerFromInput(int controller_num)
{


}

void AddJoystick(union SDL_Event *event)
{
    int id = event->jdevice.which;
    if( SDL_NumJoysticks() < 1 )
    {
        LogError("Trying to load a joystick, but none are connected.  How?");
        return;
    }
    connected_joysticks[current_joysticks] = SDL_JoystickOpen(id);
    if( connected_joysticks[current_joysticks] == NULL )
    {
        LogError( "Warning: Unable to open game controller! SDL Error: %s", SDL_GetError() );
        return;
    }
    ++current_player_controllers;

}

struct _SDL_Joystick* GetJoystick(int joystick_to_get)
{
    if(current_joysticks < joystick_to_get)
    {
        LogError("Trying to get a joystick that isn't instantiated");
        return NULL;
    }
    return connected_joysticks[joystick_to_get];
}
