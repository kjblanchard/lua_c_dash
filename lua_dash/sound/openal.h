#include <AL/al.h>
#include <AL/alext.h>
#include "alhelpers.h"
#include "vorbis/codec.h"
//#include <sndfile.h>
#include <vorbis/vorbisfile.h>

/* Define the number of buffers and buffer size (in milliseconds) to use. 4
 * buffers with 8192 samples each gives a nice per-chunk size, and lets the
 * queue last for almost one second at 44.1khz. */
#define NUM_BUFFERS 4
#define BUFFER_SAMPLES 8192


typedef struct StreamPlayer {
    /* These are the buffers and source to play out through OpenAL with */
    ALuint buffers[NUM_BUFFERS];
    ALuint source;

    /* Handle for the audio file */
    OggVorbis_File vbfile;
    vorbis_info* vbinfo;


 //   SNDFILE *sndfile;
 //   SNDFILE *sndfile2;
 //   SF_INFO sfinfo;
 //   SF_INFO sfinfo2;
    short *membuf;
    unsigned char introloop;
    unsigned char looped;

    /* The format of the output stream (sample rate is in sfinfo) */
    ALenum format;
} StreamPlayer;

StreamPlayer* play(char* filename);
int close(StreamPlayer* player);
void update(StreamPlayer* player);
