#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "debug.h"
#include "graphics_device.h"
#include <SDL2/SDL_events.h>

#ifdef DEBUG_BUILD_ENABLED
#define NK_INCLUDE_FIXED_TYPES
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#define INCLUDE_OVERVIEW
#include "../external/nuklear.h"
#include "../external/nuklear_sdl_renderer.h"
#ifdef INCLUDE_OVERVIEW
#include "../external/nuklear_overview.h"
#endif
struct nk_colorf bg;
#endif


void Log(LogLevel level, const char* thing_to_write)
{
    printf("%s\n",thing_to_write);
}
void LogWarn(const char *fmt, ...)
{
    va_list args;
    char buf[100];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Log(Sg_Debug_Warn, buf);
}
void LogError(const char *fmt, ...)
{
    va_list args;
    char buf[100];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    Log(Sg_Debug_Error, buf);
}

int InitDebugWindow()
{
#ifndef DEBUG_BUILD_ENABLED
    return 1;
#endif
#ifdef DEBUG_BUILD_ENABLED
    if(debug_window)
        return 1;
    debug_window = malloc(sizeof(GraphicsDevice));
    debug_window->debug_graphics_device = CreateDebugGraphicsDevice();
    if(!debug_window->debug_graphics_device)
        return 0;
    debug_window->ctx = nk_sdl_init(debug_window->debug_graphics_device->game_window, debug_window->debug_graphics_device->game_renderer);
    struct nk_font_atlas *atlas;
    struct nk_font_config config = nk_font_config(0);
    struct nk_font *font;
    /* set up the font atlas and add desired font; note that font sizes are
     * multiplied by font_scale to produce better results at higher DPIs */
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_default(atlas, 13 * debug_window->debug_graphics_device->font_scale, &config);
    nk_sdl_font_stash_end();
    /* this hack makes the font appear to be scaled down to the desired
     * size and is only necessary when font_scale > 1 */
    font->handle.height /= debug_window->debug_graphics_device->font_scale;
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    nk_style_set_font(debug_window->ctx, &font->handle);
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    return 1;
#endif
}

void ProcessDebugWindowInputBegin()
{
#ifndef DEBUG_BUILD_ENABLED
    return;
#endif
#ifdef DEBUG_BUILD_ENABLED
    nk_input_begin(debug_window->ctx);
#endif

}
void ProcessDebugWindowInput(SDL_Event* event)
{
#ifndef DEBUG_BUILD_ENABLED
    return;
#endif
#ifdef DEBUG_BUILD_ENABLED
    nk_sdl_handle_event(event);
#endif

}
void ProcessDebugWindowInputEnd()
{
#ifndef DEBUG_BUILD_ENABLED
    return;
#endif
#ifdef DEBUG_BUILD_ENABLED
    nk_input_end(debug_window->ctx);
#endif
}
void ProcessDebugWindowGraphics()
{
#ifndef DEBUG_BUILD_ENABLED
    return;
#endif
#ifdef DEBUG_BUILD_ENABLED

    if (nk_begin(debug_window->ctx, "Demo", nk_rect(50, 50, 230, 250),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
        enum {EASY, HARD};
        static int op = EASY;
        static int property = 20;

        nk_layout_row_static(debug_window->ctx, 30, 80, 1);
        if (nk_button_label(debug_window->ctx, "button"))
            fprintf(stdout, "button pressed\n");
        nk_layout_row_dynamic(debug_window->ctx, 30, 2);
        if (nk_option_label(debug_window->ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(debug_window->ctx, "hard", op == HARD)) op = HARD;
        nk_layout_row_dynamic(debug_window->ctx, 25, 1);
        nk_property_int(debug_window->ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(debug_window->ctx, 20, 1);
        nk_label(debug_window->ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(debug_window->ctx, 25, 1);
        if (nk_combo_begin_color(debug_window->ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(debug_window->ctx),400))) {
            nk_layout_row_dynamic(debug_window->ctx, 120, 1);
            bg = nk_color_picker(debug_window->ctx, bg, NK_RGBA);
            nk_layout_row_dynamic(debug_window->ctx, 25, 1);
            bg.r = nk_propertyf(debug_window->ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
            bg.g = nk_propertyf(debug_window->ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
            bg.b = nk_propertyf(debug_window->ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
            bg.a = nk_propertyf(debug_window->ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(debug_window->ctx);
        }
    }
    nk_end(debug_window->ctx);
    overview(debug_window->ctx);
    SDL_SetRenderDrawColor(debug_window->debug_graphics_device->game_renderer, bg.r * 255, bg.g * 255, bg.b * 255, bg.a * 255);
    SDL_RenderClear(debug_window->debug_graphics_device->game_renderer);
    nk_sdl_render(NK_ANTI_ALIASING_ON);
    SDL_RenderPresent(debug_window->debug_graphics_device->game_renderer);
#endif

}
void ShutdownDebugWindow()
{
#ifndef DEBUG_BUILD_ENABLED
    return;
#endif
#ifdef DEBUG_BUILD_ENABLED
    nk_sdl_shutdown();
#endif
}
