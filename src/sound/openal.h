#pragma once
#include <vorbis/vorbisfile.h>
#include "al.h"

typedef struct Sg_Loaded_Sfx{
    ALsizei size;
    ALenum format;
    long sample_rate;
    short* sound_data;

} Sg_Loaded_Sfx;

/**
 * @brief Initialize the openAl backend
 *
 * @return 1 if successful, 0 if not.
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
int PlayBgmAl(const char* filename, double* loop_begin, double* loop_end, float volume);
int StopBgmAl();
/**
 * @brief Pauses the playing bgm_player.
 *
 * @return 1 if successful, 0 if failed.
 */
int PauseBgmAl();
/**
 * @brief Unpauses the paused bgm_player
 *
 * @return 1 if successful, 0 if failed.
 */
int UnpauseBgmAl();
/**
 * @brief Loads a buffer full of the full sfx file, and returns it's information.
 *
 * @param filename The name to load
 *
 * @return A Sg_loaded_Sfx, that has the sound_data within it.
 */
Sg_Loaded_Sfx* LoadSfxFileAl(const char *filename);
/**
 * @brief Properly unloads a loaded sfx files memory.
 *
 * @param loaded_sfx The Loaded sfx that should be freed
 *
 * @return 1 if successful, 0 if failed.
 */
int CloseSfxFileAl(Sg_Loaded_Sfx* loaded_sfx);
int PlaySfxAl(Sg_Loaded_Sfx* sound_file, float volume);
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

