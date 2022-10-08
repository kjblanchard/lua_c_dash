#include <stdint.h>
#include <SDL2/SDL_keyboard.h>
#include "input.h"


static void InitializeKeyboardStateArrays();
static uint8_t current_keyboard_state[SDL_NUM_SCANCODES];
static uint8_t previous_keyboard_state[SDL_NUM_SCANCODES];

void InitializeInput()
{
    InitializeKeyboardStateArrays();
}
/**
 * @brief Loads the previous and current keyboard states.
 */
static void InitializeKeyboardStateArrays()
{
    memset(previous_keyboard_state, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(current_keyboard_state, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

int IsKeyHeldDown(SDL_Scancode key)
{
    return previous_keyboard_state[key] && current_keyboard_state[key];
}

int IsKeyJustPressed(const SDL_Scancode key)
{
    return current_keyboard_state[key] && !previous_keyboard_state[key];
}

int IsKeyJustReleased(const SDL_Scancode key)
{
    return !current_keyboard_state[key] && previous_keyboard_state[key];
}

void UpdateInputKeyboardStates()
{
    memcpy(previous_keyboard_state, current_keyboard_state, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(current_keyboard_state, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);
}

