#include <AL/al.h>
#include <AL/alext.h>
#include <vorbis/vorbisfile.h>
#include "alhelpers.h"

#define BGM_NUM_BUFFERS 4
#define BGM_BUFFER_SAMPLES 8192 //8kb
#define VORBIS_REQUEST_SIZE 4096 //Max size to request from vorbis to load.


/**
 * @brief The BGM streaming player.  Probably only need one of these at any time
 *
 */
typedef struct StreamPlayer {
    /**
     * @brief Array of buffers to use for referencing the buffer_id in openal
     */
    ALuint buffers[BGM_NUM_BUFFERS];
    /**
     * @brief The source number for calls.
     */
    ALuint source;
    ogg_int64_t loop_point_begin;
    ogg_int64_t loop_point_end;
    ogg_int64_t total_bytes_read_this_loop;
    OggVorbis_File vbfile;
    vorbis_info* vbinfo;
    short *membuf;
    ALenum format;
} StreamPlayer;


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
int PlayBgm(char* filename, double loop_begin, double loop_end);
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
