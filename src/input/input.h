/**
 * @file input.h
 * @brief Handles the core input gathering each frame.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-07
 */
#pragma once
#include <SDL2/SDL_scancode.h>

/**
 * @brief Initializes the Engines input.
 */
void InitializeInput();
/**
 * @brief Checks to see if a key on the keyboard is held.
 *
 * @param key The key to check
 *
 * @return 1 if true, 0 if false.
 */
int IsKeyHeldDown(const SDL_Scancode key);
/**
 * @brief Checks to see if a key on the keyboard is pressed
 *
 * @param key The key to check.
 *
 * @return 1 of true, 0 if false
 */
int IsKeyJustPressed(const SDL_Scancode key);
/**
 * @brief Checks to see if a key on the keyboard is released
 *
 * @param key The key to check.
 *
 * @return 1 if true, 0 if false.
 */
int IsKeyJustReleased(const SDL_Scancode key);
/**
 * @brief Gets the current keyboard states, and loads the previous keyboard states.  This should be called every update frame after events are processed.
 */
void UpdateInputKeyboardStates();
