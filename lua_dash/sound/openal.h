#include <AL/al.h>
#include <AL/alext.h>
#include "alhelpers.h"
#include "vorbis/codec.h"
#include <vorbis/vorbisfile.h>

/* Define the number of buffers and buffer size (in milliseconds) to use. 4
 * buffers with 8192 samples each gives a nice per-chunk size, and lets the
 * queue last for almost one second at 44.1khz. */
#define NUM_BUFFERS 4
#define BUFFER_SAMPLES 8192
#define VORBIS_REQUEST_SIZE 4096


typedef struct StreamPlayer {
    ALuint buffers[NUM_BUFFERS];
    ALuint source;
    ogg_int64_t loop_point_begin;
    ogg_int64_t loop_point_end;
    ogg_int64_t total_bytes_read_this_loop;
    OggVorbis_File vbfile;
    vorbis_info* vbinfo;
    short *membuf;
    ALenum format;
} StreamPlayer;


int Initialize();
int PlayBgm(char* filename, double loop_begin, double loop_end);
void Update();

int close();
void update();
