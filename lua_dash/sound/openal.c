#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogg/os_types.h>
#include <vorbis/vorbisfile.h>
#include "openal.h"

static StreamPlayer* bgm_player;

static StreamPlayer *NewPlayer(void);
static int PreBakeBgm(char* filename, double loop_begin, double loop_end);
static void DeletePlayer(StreamPlayer *player);
static int OpenPlayerFile(StreamPlayer *player, const char *filename, double loop_begin, double loop_end);
static void GetLoopPoints(StreamPlayer*player, double* begin, double*end);
static void ClosePlayerFile(StreamPlayer *player);
static int StartPlayer(StreamPlayer *player);
static int UpdatePlayer(StreamPlayer *player);

int InitializeAl()
{
    StreamPlayer *player;
    if (InitAL() != 0)
        return 0; // Error
    bgm_player = NewPlayer();
    return 1;
}

int PlayBgm(char* filename, double loop_begin, double loop_end)
{
    PreBakeBgm(filename,loop_begin,loop_end);
    if (!StartPlayer(bgm_player))
    {
        ClosePlayerFile(bgm_player);
        return 0;
    }
    return 1;

}

static int PreBakeBgm(char* filename, double loop_begin, double loop_end)
{
    const char *namepart;
    if (!OpenPlayerFile(bgm_player,filename, loop_begin, loop_end))
        return 0;

    /* Get the name portion, without the path, for display. */
    namepart = strrchr(filename, '/');
    if (namepart || (namepart = strrchr(filename, '\\')))
        namepart++;
    else
        namepart = filename;

    //TODO convert this to debug
    printf("Playing: %s (%s, %ldhz)\n", namepart, FormatName(bgm_player->format),
            bgm_player->vbinfo->rate);
    fflush(stdout);
    return 1;

}

/**
 * @brief Opens a new file and sets up the loop points on the player.  Closes the current file.
 *
 * @param player The bgm_player that this should be performed on
 * @param filename The filename that should be read
 * @param loop_begin The point that we should start looping
 * @param loop_end The point we should end looping
 *
 * @return 
 */
static int OpenPlayerFile(StreamPlayer *player, const char *filename, double loop_begin, double loop_end)
{
    ClosePlayerFile(player);
    int result = ov_fopen(filename, &player->vbfile);
    if (result !=0)
    {
        fprintf(stderr, "Could not open audio in %s: %d\n", filename, result);
        return 0;
    }
    player->vbinfo = ov_info(&player->vbfile, -1);
    if (player->vbinfo->channels == 1)
    {
        player->format = AL_FORMAT_MONO16;
    }
    else
    {
        player->format = AL_FORMAT_STEREO16;
    }
    if (!player->format)
    {
        fprintf(stderr, "Unsupported channel count: %d\n", player->vbinfo->channels);
        ov_clear(&player->vbfile);
        return 0;
    }
    //TODO can we actually load more here?  Seems like our buffers arent fully loading for some reason.
    size_t data_read_size = (size_t)(BGM_BUFFER_SAMPLES);
    player->membuf = malloc(data_read_size);

    GetLoopPoints(bgm_player, &loop_begin, &loop_end);
    return 1;
}

static void GetLoopPoints(StreamPlayer*player, double* loop_begin, double* loop_end)
{
    unsigned char not_at_beginning = 0;
    if(loop_begin)
    {
        ov_time_seek(&player->vbfile, *loop_begin);
        player->loop_point_begin = ov_pcm_tell(&player->vbfile);
        not_at_beginning = 1;
    }
    else
        player->loop_point_begin = ov_pcm_tell(&player->vbfile);
    //Loop end needs to be measured against our buffers loading, so they will be multiplied by channels and sizeof.
    //Due to us checking this on every step.
    if(loop_end)
    {
        ov_time_seek(&player->vbfile, *loop_end);
        player->loop_point_end = ov_pcm_tell(&player->vbfile) * player->vbinfo->channels * sizeof(short);
        not_at_beginning = 1;
    }
    else
        player->loop_point_end = ov_pcm_total(&player->vbfile,-1) * player->vbinfo->channels * sizeof(short);
    if(not_at_beginning)
        ov_raw_seek(&player->vbfile, 0);
}

void UpdateAl()
{
    UpdatePlayer(bgm_player);

}

/* Creates a new player object, and allocates the needed OpenAL source and
 * buffer objects. Error checking is simplified for the purposes of this
 * example, and will cause an abort if needed. */
static StreamPlayer *NewPlayer(void)
{
    StreamPlayer *player;
    //Init memory for player and vorbisfile
    player = calloc(1, sizeof(*player));
    //    player->vbfile = calloc(1,sizeof(OggVorbis_File));
    assert(player != NULL);


    /* Generate the buffers and source */
    alGenBuffers(BGM_NUM_BUFFERS, player->buffers);
    assert(alGetError() == AL_NO_ERROR && "Could not create buffers");

    alGenSources(1, &player->source);
    assert(alGetError() == AL_NO_ERROR && "Could not create source");

    /* Set parameters so mono sources play out the front-center speaker and
     * won't distance attenuate. */
    alSource3i(player->source, AL_POSITION, 0, 0, -1);
    alSourcei(player->source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(player->source, AL_ROLLOFF_FACTOR, 0);
    assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");

    return player;
}

typedef enum BufferFillFlags {
    Buff_Fill_Default,
    Buff_Fill_MusicEnded,
    Buff_Fill_MusicHitLoopPoint
} BufferFillFlags;

/* Destroys a player object, deleting the source and buffers. No error handling
 * since these calls shouldn't fail with a properly-made player object. */
static void DeletePlayer(StreamPlayer *player)
{
    ClosePlayerFile(player);

    alDeleteSources(1, &player->source);
    alDeleteBuffers(BGM_NUM_BUFFERS, player->buffers);
    if (alGetError() != AL_NO_ERROR)
        fprintf(stderr, "Failed to delete object IDs\n");

    memset(player, 0, sizeof(*player));
    free(player);
}


/**
 * @brief Deallocates the current memory buffer, so that it can be used for another song.
 *
 * @param player The bgm stream to clear.
 */
static void ClosePlayerFile(StreamPlayer *player)
{
    free(player->membuf);
    player->membuf = NULL;
}

static long LoadBufferData(StreamPlayer* player, BufferFillFlags* buff_flags)
{
    //Set the buffer flags to 0, as it is normal
    *buff_flags = 0;
    //Set the bytes read to 0, since we didn't read any bytes yet
    long total_buffer_bytes_read = 0;
    //Set the max request size to get data from the vorbis file
    int request_size = VORBIS_REQUEST_SIZE;
    //Our goal is to read enough bytes to fill up our buffer samples (8kbs), so while we have read less than that, keep loading.
    //This is due to vorbis reading random amounts, and not the whole size at once.
    while(total_buffer_bytes_read < BGM_BUFFER_SAMPLES)
    {
        //Update the request size.  Remember our goal is to read the full buffer.
        request_size = (total_buffer_bytes_read + request_size <= BGM_BUFFER_SAMPLES) 
            ? request_size 
            : BGM_BUFFER_SAMPLES - total_buffer_bytes_read;
        //Update the request size.  Remember we don't want to go past the loop end point.
        request_size = (total_buffer_bytes_read + request_size + player->total_bytes_read_this_loop <= player->loop_point_end) 
            ? request_size 
            : player->loop_point_end - (total_buffer_bytes_read + player->total_bytes_read_this_loop);
        if(request_size == 0)
        {
            *buff_flags = Buff_Fill_MusicHitLoopPoint;
            break;
            //We are at the end of the loop point.
        }
        //Actually read from the file.Notice we offset our memory location(membuf) by the amount of bytes read so that we keep loading more.
        int current_pass_bytes_read = ov_read(&player->vbfile, (char*)player->membuf + total_buffer_bytes_read, request_size, 0, sizeof(short), 1, 0);
        //If we have read 0 bytes, we are at the end of the song.
        if(current_pass_bytes_read == 0)
        {
            //Set the buffer flags to ended
            *buff_flags = Buff_Fill_MusicEnded;
            //Stop filling the buffer and exit the while loop
            break;
        }
        //Update the amount of bytes read for this buffer
        total_buffer_bytes_read += current_pass_bytes_read;
    }
    //Add the bytes read to the current bytes read for the entire loop, used for tracking the current loading point.
    player->total_bytes_read_this_loop += total_buffer_bytes_read;

    return total_buffer_bytes_read;
}

static int StartPlayer(StreamPlayer *player)
{
    ALsizei i;

    /* Rewind the source position and clear the buffer queue */
    alSourceRewind(player->source);
    alSourcei(player->source, AL_BUFFER, 0);

    /* Fill the buffer queue */
    BufferFillFlags buf_flags;
    for (i = 0; i < BGM_NUM_BUFFERS; i++)
    {
        long bytes_read = LoadBufferData(player, &buf_flags);
        alBufferData(player->buffers[i], player->format, player->membuf, (ALsizei)bytes_read,
                player->vbinfo->rate);
    }

    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error buffering for playback\n");
        return 0;
    }

    /* Now queue and start playback! */
    alSourceQueueBuffers(player->source, i, player->buffers);
    alSourcePlay(player->source);
    ALint buffers = 0;
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error starting playback\n");
        return 0;
    }

    return 1;
}
/**
 * @brief Restart the stream from the loop point.
 *
 * @param player Pointer to a stream player
 *
 * @return 
 */
static int RestartStream(StreamPlayer *player)
{
    ov_pcm_seek_lap(&player->vbfile, player->loop_point_begin);
    player->total_bytes_read_this_loop = ov_pcm_tell(&player->vbfile) * player->vbinfo->channels * sizeof(short);
    return 0;
}

static int UpdatePlayer(StreamPlayer *player)
{
    ALint processed, state;
    /* Get relevant source info */
    alGetSourcei(player->source, AL_SOURCE_STATE, &state);
    alGetSourcei(player->source, AL_BUFFERS_PROCESSED, &processed);
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error checking source state\n");
        return 0;
    }

    /* Unqueue and handle each processed buffer */
    static int buffs_processed = 0;
    while (processed > 0)
    {
        ALuint bufid;
        alSourceUnqueueBuffers(player->source, 1, &bufid);
        processed--;
        BufferFillFlags buf_flags = 0;
        long bytes_read = LoadBufferData(player, &buf_flags);
        alBufferData(bufid, player->format, player->membuf, (ALsizei)bytes_read,
                player->vbinfo->rate);
        alSourceQueueBuffers(player->source, 1, &bufid);
        if (alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Error buffering data\n");
            return 0;
        }
        if(buf_flags == Buff_Fill_MusicEnded || buf_flags == Buff_Fill_MusicHitLoopPoint)
        {
                RestartStream(player);
        }
    }

    /* Make sure the source hasn't underrun */
    if (state != AL_PLAYING && state != AL_PAUSED)
    {
        puts("Just hit underrun, and we really shouldn't");
        ALint queued;

        /* If no buffers are queued, playback is finished or starved */
        alGetSourcei(player->source, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0)
            return 0;

        alSourcePlay(player->source);
        if (alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Error restarting playback\n");
            return 0;
        }
    }

    return 1;
}

int CloseAl()
{
    ClosePlayerFile(bgm_player);
    DeletePlayer(bgm_player);
    bgm_player = NULL;
    CloseAL();
    return 0;
}
