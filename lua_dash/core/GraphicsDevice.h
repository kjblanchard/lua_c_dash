#pragma once
#include "../base/Point.h"

struct SDL_Window;
struct SDL_Renderer;

typedef struct GraphicsDevice
{
    Point world_size;
    Point window_size;
    struct SDL_Window* game_window;
    struct SDL_Renderer* game_renderer;

} GraphicsDevice;

GraphicsDevice* CreateGraphicsDevice();
void DestroyGraphicsDevice(GraphicsDevice* graphics);

