/**
 * @file debug.h
 * @brief Holds the debug window/renderer (Nuklear) and also has some simple logging functions that Should be used.
 * @author Kevin Blanchard
 * @version 0.1
 * @date 2022-09-18
 */

#pragma once
struct GraphicsDevice;
union SDL_Event;


/**
 * @brief The different log levels.  Mostly used internally.
 *
 */
typedef enum LogLevel
{
    Sg_Debug_Default,
    Sg_Debug_Debug,
    Sg_Debug_Info,
    Sg_Debug_Warn,
    Sg_Debug_Error,
} LogLevel;

/**
 * @brief The debug window, and it's nuklear context
 */
typedef struct DebugWindow
{
    struct GraphicsDevice* debug_graphics_device;
    struct nk_context *ctx;

} DebugWindow;

/**
 * @brief The global debug_window, that has a graphics device and context.
 */
extern DebugWindow* debug_window;
/**
 * @brief The current games logging level, this should be set inside of lua.
 */
extern LogLevel game_log_level ;
/**
 * @brief Opens and/or creates a file for debug logging.
 *
 * @return 1 if successful, 0 if failed.
 */
int InitializeDebugLogFile();
/**
 * @brief Closes the open file for logging.
 *
 * @return 
 */
int CloseDebugLogFile();
/**
 * @brief Log a Debug log in a printf format
 *
 * @param format The printf styped text
 * @param ... The variables for the printf
 */
void LogDebug(const char *format, ...);
/**
 * @brief Log a warning log in a printf format
 *
 * @param format The printf styped text
 * @param ... The variables for the printf
 */
void LogInfo(const char *format, ...);
/**
 * @brief Log a warning log in a printf format
 *
 * @param format The printf styped text
 * @param ... The variables for the printf
 */
void LogWarn(const char *format, ...);
/**
 * @brief Log a error log in a printf format
 *
 * @param format The printf styped text
 * @param ... The variables for the printf
 */
void LogError(const char *format, ...);

/**
 * @brief Initializes the Debug window and renderer to be used
 *
 * @return 
 */
int InitDebugWindow();
void ToggleDebugWindow(int enabled);
void ProcessDebugWindowInputBegin();
void ProcessDebugWindowInput(union SDL_Event* event);
void ProcessDebugWindowInputEnd();
void ProcessDebugWindowGraphics();
void ShutdownDebugWindow();
