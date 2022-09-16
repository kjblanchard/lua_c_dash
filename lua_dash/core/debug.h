#pragma once
/**
 * @brief The different log levels.  Mostly used internally.
 *
 */
typedef enum LogLevel
{
    Sg_Debug_Default,
    Sg_Debug_Debug,
    Sg_Debug_Warn,
    Sg_Debug_Error,
} LogLevel;
/**
 * @brief The internal logging function that the others will end up calling.  Probably don't call it manually
 *
 * @param level The log level to log this as.
 * @param data_to_write The data to write to the log.
 */
void Log(LogLevel level, const char *data_to_write);
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
