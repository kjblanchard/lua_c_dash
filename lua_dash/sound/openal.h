#pragma once

#include <AL/al.h>
#include <AL/alext.h>
#include <vorbis/vorbisfile.h>
#include "alhelpers.h"

/**
 * @brief Initialize the openAl backend
 *
 * @return 
 */
int InitializeAl();
/**
 * @brief Play a streaming BGM.
 *
 * @param filename The file to stream from
 * @param loop_begin The point the loop should begin when looping, 0 means beginning.
 * @param loop_end The point the loop should end and move to the loop)begin.  0 means end of file.
 *
 * @return 1 on Success, 0 on failure.
 */
int PlayBgmAl(char* filename, double* loop_begin, double* loop_end);
/**
 * @brief Updates the openal sound system.
 */
void UpdateAl();
/**
 * @brief Closes the AL sound system.
 *
 * @return 1 on Success, 0 on failure.
 */
int CloseAl();
