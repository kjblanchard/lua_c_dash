/**
 * @file graphics_device.h
 * @brief Handles Holding the wondows and renderers.
 * @author Kevin Blanchard
 * @version 0.1
 * @date 2022-09-17
 */
#pragma once
#include "../base/point.h"

struct SDL_Window;
struct SDL_Renderer;

/**
 * @brief A type to hold the windows and renderers for the game.
 */
typedef struct GraphicsDevice
{
    Point world_size;
    Point window_size;
    //Nuklear thing.
    float font_scale;
    struct SDL_Window* game_window;
    struct SDL_Renderer* game_renderer;
    int window_id;

} GraphicsDevice;

/**
 * @brief Creates the graphics device
 *
 * @return  Pointer to the created graphics device
 */
GraphicsDevice* CreateGraphicsDevice();
/**
 * @brief Creates the graphics device
 *
 * @return  Pointer to the created graphics device
 */
GraphicsDevice* CreateDebugGraphicsDevice();
/**
 * @brief Destroys a graphics device
 *
 * @param graphics the graphics device to destroy.
 */
void DestroyGraphicsDevice(GraphicsDevice* graphics);

