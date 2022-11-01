#include <stdint.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include "../debug/debug.h"
#include "player_controller.h"
#include "input.h"

#define MAX_PLAYERS 4
static int current_player_controllers = 0;
static PlayerController* player_controllers[MAX_PLAYERS];

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
        PlayerControllerInputReceive(player_controllers[i], event);
    }

}

void RemovePlayerControllerFromInput(int controller_num)
{


}
