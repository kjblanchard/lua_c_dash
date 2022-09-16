#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

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
