/**
 * @file sound.h
 * @brief High level wrapper over the openal.h commands to easily play BGM and SFX
 * @author Kevin Blanchard
 * @version 0.1
 * @date 2022-09-17
 */

#pragma once
/**
 * @brief Load the Sound backend, this must be called before any other functions are available.
 *
 * @return 
 */
int InitializeSound();
/**
 * @brief Play a specific BGM.  It will loop continuously until you call the Stop function on it.  Its loop points and number are determined by the config file.
 *
 * @param bgm_number The current bgm number to play.
 * @param volume The volume we want to set this to.  1 is regular volume.
 *
 * @return 1 if Successful, 0 if failed to start.
 */
int PlayBgm(int bgm_number, float volume);
/**
 * @brief Stops a playing bgm.  If stop_at_end is true, then it will stop playing at the end of the song.
 *
 * @param stop_at_end If sent 1, this will stop at the end of the song instead of now.
 *
 * @return 1 if successful, 0 if failed.
 */
int StopBgm(int stop_at_end);
/**
 * @brief Plays a Sound effect once in its own buffer.  There is only a total of 10 buffers available for playing at a time.
 *
 * @param sfx_number The Sound effect to play
 *
 * @return 1 if successful, 0 if failed to start
 */
int PlaySfxOneShot(int sfx_number, float volume);
/**
 * @brief Preloads a sfx sound.  
 *
 * @param sfx_number the sfx number to load.
 *
 * @return Returns 1 if it was loaded or already loaded, and 0 if load failed.
 */
int LoadSfx(int sfx_number);
/**
 * @brief Unloads a loaded sound
 *
 * @param sfx_number The number sfx that we should unload
 *
 * @return 1 if it was unloaded or wasn't loaded already, and 0 if it was not null
 */
int UnloadSfx(int sfx_number);
/**
 * @brief This should be called every frame.  Updates the BGM sound and such.
 */
void UpdateSound();
/**
 * @brief Closes openal and destroys all bgm and sfx.
 */
void CloseSound();

