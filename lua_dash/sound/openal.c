#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogg/os_types.h>
#include <vorbis/vorbisfile.h>
#include "AL/al.h"
#include "openal.h"
#include "vorbis/codec.h"

#define BGM_NUM_BUFFERS 4
#define MAX_SFX_SOUNDS 10
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
    ALuint source;
    ogg_int64_t loop_point_begin;
    ogg_int64_t loop_point_end;
    ogg_int64_t total_bytes_read_this_loop;
    OggVorbis_File vbfile;
    vorbis_info* vbinfo;
    short *membuf;
    ALenum format;
} StreamPlayer;

typedef struct SfxPlayer {
    ALuint buffers[MAX_SFX_SOUNDS];
    ALuint sources[MAX_SFX_SOUNDS];
    ALuint free_buffers;

} SfxPlayer;



/**
 * @brief The different ways that our buffer can fill.
 */
typedef enum BufferFillFlags
{
    Buff_Fill_Default,
    Buff_Fill_MusicEnded,
    Buff_Fill_MusicHitLoopPoint
} BufferFillFlags;

static StreamPlayer* bgm_player;
static SfxPlayer* sfx_player;
static StreamPlayer* NewPlayer(void);
static SfxPlayer* NewSfxPlayer();
static int PreBakeBgm(StreamPlayer* player, char* filename, double* loop_begin, double* loop_end);
static int PreBakeBuffers(StreamPlayer* player);
static void DeletePlayer(StreamPlayer* player);
static int OpenPlayerFile(StreamPlayer* player, const char* filename, double* loop_begin, double* loop_end);
static void GetLoopPoints(StreamPlayer* player, double* begin, double* end);
static long LoadBufferData(StreamPlayer* player, BufferFillFlags* buff_flags);
static int HandleProcessedBuffer(StreamPlayer* player);
static void ClosePlayerFile(StreamPlayer *player);
static int StartPlayer(StreamPlayer *player);
static int UpdatePlayer(StreamPlayer *player);
static int UpdateSfxPlayer(SfxPlayer *player);
static int RestartStream(StreamPlayer *player);
static Sg_Loaded_Sfx* LoadSfxFile(SfxPlayer* player, const char *filename);
static int PlaySfxFile(SfxPlayer* player, Sg_Loaded_Sfx* loaded_sfx);


/**
 * @brief Initialize the OpenAl backend, and create the static players.
 *
 * @return 
 */
int InitializeAl()
{
    StreamPlayer* player;
    if (InitAL() != 0)
        return 0; // Error
    bgm_player = NewPlayer();
    sfx_player = NewSfxPlayer();
    return 1;
}

/**
 * @brief Constructor for a BgmPlayer
 *
 * @return A initialized bgmplayer
 */
static StreamPlayer* NewPlayer(void)
{
    StreamPlayer* player;
    player = calloc(1, sizeof(*player));
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

static SfxPlayer* NewSfxPlayer()
{
    SfxPlayer* sfx_player;
    sfx_player = calloc(1, sizeof(*sfx_player));
    alGenBuffers(MAX_SFX_SOUNDS, sfx_player->buffers);
    assert(alGetError() == AL_NO_ERROR && "Could not create buffers");
    alGenSources(MAX_SFX_SOUNDS, sfx_player->sources);
    assert(alGetError() == AL_NO_ERROR && "Could not create source");
    for (size_t i = 0; i < 10; ++i) {
        alSource3i(sfx_player->sources[i], AL_POSITION, 0, 0, -1);
        alSourcei(sfx_player->sources[i], AL_SOURCE_RELATIVE, AL_TRUE);
        alSourcei(sfx_player->sources[i], AL_ROLLOFF_FACTOR, 0);
        assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");
    }
    sfx_player->free_buffers = MAX_SFX_SOUNDS;
    return sfx_player;

}

int PlaySfxAl(Sg_Loaded_Sfx* sound_file)
{
    PlaySfxFile(sfx_player,sound_file);
    return 1;

}
static int PlaySfxFile(SfxPlayer* player, Sg_Loaded_Sfx* sfx_file)
{
    if(player->free_buffers == 0)
        return 0;
    int buffer_num = --player->free_buffers;
    alSourceRewind(sfx_player->sources[buffer_num]);
    alSourcei(sfx_player->sources[buffer_num], AL_BUFFER, 0);
    alBufferData(sfx_player->buffers[buffer_num], sfx_file->format, sfx_file->sound_data, sfx_file->size, sfx_file->sample_rate);
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error adding bufferdata\n");
        return 0;
    }
    alSourceQueueBuffers(sfx_player->sources[buffer_num], 1, &sfx_player->buffers[buffer_num]);
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error queing buffers\n");
        return 0;
    }
    alSourcePlay(sfx_player->sources[buffer_num]);
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error playing \n");
        return 0;
    }
    

    return 1;
}

int PlayBgmAl(char* filename, double* loop_begin, double* loop_end)
{
    PreBakeBgm(bgm_player,filename,loop_begin,loop_end);
    if (!StartPlayer(bgm_player))
    {
        ClosePlayerFile(bgm_player);
        return 0;
    }
    return 1;

}

static int PreBakeBgm(StreamPlayer* player, char* filename, double* loop_begin, double* loop_end)
{
    const char* namepart;
    if (!OpenPlayerFile(bgm_player,filename, loop_begin, loop_end))
        return 0;
    alSourceRewind(player->source);
    alSourcei(player->source, AL_BUFFER, 0);
    PreBakeBuffers(player);
    return 1;
}

int PreBakeBuffers(StreamPlayer* player)
{
    ALsizei i;
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
static int OpenPlayerFile(StreamPlayer *player, const char *filename, double *loop_begin, double *loop_end)
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

    GetLoopPoints(bgm_player, loop_begin, loop_end);
    return 1;
}
Sg_Loaded_Sfx* LoadSfxFileAl(const char *filename)
{

    return LoadSfxFile(sfx_player, filename);

}

static Sg_Loaded_Sfx* LoadSfxFile(SfxPlayer* player, const char *filename)
{
    //TODO Close a sfx_player
    //ClosePlayerFile(player);
    //
    vorbis_info* vbinfo;
    OggVorbis_File vbfile;
    Sg_Loaded_Sfx* loaded_sfx;
    loaded_sfx = calloc(1, sizeof(*loaded_sfx));

    int result = ov_fopen(filename, &vbfile);
    if (result !=0)
    {
        fprintf(stderr, "Could not open audio in %s: %d\n", filename, result);
        return 0;
    }
    vbinfo = ov_info(&vbfile, -1);
    if (vbinfo->channels == 1)
    {
        loaded_sfx->format = AL_FORMAT_MONO16;
    }
    else
    {
        loaded_sfx->format = AL_FORMAT_STEREO16;
    }
    if (!loaded_sfx->format)
    {
        fprintf(stderr, "Unsupported channel count: %d\n", vbinfo->channels);
        ov_clear(&vbfile);
        return 0;
    }
    loaded_sfx->sample_rate = vbinfo->rate;

    //Get the size of the file in pcm.
     loaded_sfx->size =  ov_pcm_total(&vbfile,-1) * vbinfo->channels * sizeof(short);
    loaded_sfx->sound_data = malloc(loaded_sfx->size);
    int total_buffer_bytes_read = 0;
    int fully_loaded = 0;
    while(!fully_loaded)
    {
        int bytes_read =  ov_read(&vbfile, (char*)loaded_sfx->sound_data + total_buffer_bytes_read, VORBIS_REQUEST_SIZE, 0, sizeof(short), 1, 0);
        total_buffer_bytes_read += bytes_read;
        if(bytes_read == 0)
            fully_loaded = 1;
    }
    ov_clear(&vbfile);
    return loaded_sfx;

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

/**
 * @brief Sets the source to playing, which starts to play the queued buffers.
 *
 * @param player The bgm player to start.
 *
 * @return 
 */
static int StartPlayer(StreamPlayer *player)
{
    alSourcePlay(player->source);
    ALint buffers = 0;
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error starting playback\n");
        return 0;
    }
    return 1;
}

void UpdateAl()
{
    UpdatePlayer(bgm_player);
    UpdateSfxPlayer(sfx_player);

}

static int UpdatePlayer(StreamPlayer *player)
{
    ALint processed_buffers, state;
    alGetSourcei(player->source, AL_SOURCE_STATE, &state);
    alGetSourcei(player->source, AL_BUFFERS_PROCESSED, &processed_buffers);
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error checking source state\n");
        return 0;
    }

    while (processed_buffers > 0)
    {
        HandleProcessedBuffer(player);
        --processed_buffers;
    }

    //TODO handle this differently.
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
static int UpdateSfxPlayer(SfxPlayer *player)
{
    ALint processed_buffers, state;
    for (size_t i = player->free_buffers; i < MAX_SFX_SOUNDS; ++i) 
    {
        alGetSourcei(player->sources[i], AL_SOURCE_STATE, &state);
        alGetSourcei(player->sources[i], AL_BUFFERS_PROCESSED, &processed_buffers);
        if (alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Error checking source state\n");
            return 0;
        }
        while (processed_buffers > 0)
        {
            alSourceUnqueueBuffers(player->sources[i], 1, &player->buffers[i]);
            ++player->free_buffers;
            --processed_buffers;
        }
    }

    return 1;

}

/**
 * @brief Unqueue and handle each buffer that needs processing.
 *
 * @param player The strem player to get info from
 * @param processed How many processed buffers to handle
 *
 * @return 1 for Success, and 0 for failure.
 */
static int HandleProcessedBuffer(StreamPlayer* player)
{
    ALuint bufid;
    alSourceUnqueueBuffers(player->source, 1, &bufid);
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
    return 1;
}

/**
 * @brief Handles Fully loading a buffer, and setting flags for if we have reached the end of the song or a loop point.
 *
 * @param player The bgm_player to perform this on
 * @param buff_flags the buffer flags that will be modified with the result.
 *
 * @return The amount of bytes that was read from the file.
 */
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

int CloseAl()
{
    ClosePlayerFile(bgm_player);
    DeletePlayer(bgm_player);
    bgm_player = NULL;
    CloseAL();
    return 0;
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

/**
 * @brief Cleans up a BGM player and releases memory
 *
 * @param player The bgm player to destroy
 */
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
