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
 *
 * @return 1 if Successful, 0 if failed to start.
 */
int PlayBgm(int bgm_number);
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
int PlaySfxOneShot(int sfx_number);
/**
 * @brief Loads the sound section from the config file.  This will grab the sound number, name, and loop points and add them to an internal array.
 *
 * @return 1 if successfully loaded, 0 if failed.
 */
int LoadSoundConfigFile();
/**
 * @brief This should be called every frame.  Updates the BGM sound and such.
 */
void Update();


