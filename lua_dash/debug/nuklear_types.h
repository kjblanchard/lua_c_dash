
#pragma once

typedef struct NuklearLabel
{
    const char* label_name;
    float volume;
    void(*func_to_run)();

} NuklearLabel;
