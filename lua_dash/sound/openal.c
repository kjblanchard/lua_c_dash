#include "openal.h"
#include "AL/al.h"
#include "ogg/os_types.h"
#include "vorbis/vorbisfile.h"
/*
 * OpenAL Audio Stream Example
 *
 * Copyright (c) 2011 by Chris Robinson <chris.kcat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* This file contains a relatively simple streaming audio player. */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static StreamPlayer *NewPlayer(void);
static void DeletePlayer(StreamPlayer *player);
static int OpenPlayerFile(StreamPlayer *player, const char *filename);
static void ClosePlayerFile(StreamPlayer *player);
static int StartPlayer(StreamPlayer *player);
static int UpdatePlayer(StreamPlayer *player);

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

    player->loop_point_begin = 5.176;

    /* Generate the buffers and source */
    alGenBuffers(NUM_BUFFERS, player->buffers);
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
    alDeleteBuffers(NUM_BUFFERS, player->buffers);
    if (alGetError() != AL_NO_ERROR)
        fprintf(stderr, "Failed to delete object IDs\n");

    memset(player, 0, sizeof(*player));
    free(player);
}

/* Opens the first audio stream of the named file. If a file is already open,
 * it will be closed first. */
static int OpenPlayerFile(StreamPlayer *player, const char *filename)
{
    size_t data_read_size;

    ClosePlayerFile(player);

    int result = ov_fopen(filename, &player->vbfile);
    if (result !=0)
    {
        fprintf(stderr, "Could not open audio in %s: %d\n", filename, result);
        return 0;
    }
    player->vbinfo = ov_info(&player->vbfile, -1);

    /* Get the sound format, and figure out the OpenAL format */
    if (player->vbinfo->channels == 1)
        player->format = AL_FORMAT_MONO16;
    else if (player->vbinfo->channels == 2)
        player->format = AL_FORMAT_STEREO16;
    if (!player->format)
    {
        fprintf(stderr, "Unsupported channel count: %d\n", player->vbinfo->channels);
        ov_clear(&player->vbfile);
        //player->sndfile = NULL;
        return 0;
    }
    // data_read_size is the size of data to read, allocate said data space
    // this is calculated by (samples * channels * 2 (aka 16bits))
    //data_read_size = (size_t)(BUFFER_SAMPLES * player->vbinfo->channels) * sizeof(short);
    data_read_size = (size_t)(BUFFER_SAMPLES);
    printf("Data reading size is %ld bytes \n",data_read_size);
    player->membuf = malloc(data_read_size);


    //Figure out the end loop point. For now, we will use the end of the song.
    return 1;
}

/* Closes the audio file stream */
static void ClosePlayerFile(StreamPlayer *player)
{
    //    if (&player->vbfile)
    //        ov_clear(&player->vbfile);
    //    player->vbfile = NULL;

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
    while(total_buffer_bytes_read < BUFFER_SAMPLES)
    {
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
        //Update the request size.  Remember our goal is to read the full buffer.
        request_size = (total_buffer_bytes_read + request_size <= BUFFER_SAMPLES) ? request_size : BUFFER_SAMPLES - total_buffer_bytes_read;
    }

    return total_buffer_bytes_read;
}

/* Prebuffers some audio from the file, and starts playing the source */
static int StartPlayer(StreamPlayer *player)
{
    ALsizei i;

    /* Rewind the source position and clear the buffer queue */
    alSourceRewind(player->source);
    alSourcei(player->source, AL_BUFFER, 0);

    /* Fill the buffer queue */
    BufferFillFlags buf_flags;
    for (i = 0; i < NUM_BUFFERS; i++)
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
static int RestartStream(StreamPlayer *player, ALuint bufid)
{
    ov_time_seek_lap(&player->vbfile, player->loop_point_begin);
    alBufferData(bufid, player->format, player->membuf, (ALsizei)0,
            player->vbinfo->rate);
    alSourceQueueBuffers(player->source, 1, &bufid);

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
        printf("Buff was processed, %d\n",++buffs_processed);
        ALuint bufid;
        alSourceUnqueueBuffers(player->source, 1, &bufid);
        processed--;
        BufferFillFlags buf_flags = 0;
        long bytes_read = LoadBufferData(player, &buf_flags);
        switch (buf_flags) 
        {
            case Buff_Fill_Default:
                alBufferData(bufid, player->format, player->membuf, (ALsizei)bytes_read,
                        player->vbinfo->rate);
                alSourceQueueBuffers(player->source, 1, &bufid);
                if (alGetError() != AL_NO_ERROR)
                {
                    fprintf(stderr, "Error buffering data\n");
                    return 0;
                }
                break;
            case Buff_Fill_MusicEnded:
                RestartStream(player, bufid);
                break;
            case Buff_Fill_MusicHitLoopPoint:
                break;
        }
    }

    /* Make sure the source hasn't underrun */
    if (state != AL_PLAYING && state != AL_PAUSED)
    {
        printf("Hit underrun, cause state is %d",state);
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

StreamPlayer *play(char *filename)
{
    char **filename2 = &filename;
    int files_to_load_cound = 1;
    StreamPlayer *player;
    int i;

    //TODO change InitAL's signature
    if (InitAL(&filename2, &files_to_load_cound) != 0)
        return NULL; // Error

    player = NewPlayer();

    const char *namepart;

    if (!OpenPlayerFile(player, filename))
        return NULL;

    /* Get the name portion, without the path, for display. */
    namepart = strrchr(filename, '/');
    if (namepart || (namepart = strrchr(filename, '\\')))
        namepart++;
    else
        namepart = filename;

    printf("Playing: %s (%s, %ldhz)\n", namepart, FormatName(player->format),
            player->vbinfo->rate);
    fflush(stdout);

    if (!StartPlayer(player))
    {
        ClosePlayerFile(player);
        return NULL;
    }

    return player;

}
void update(StreamPlayer *player)
{
    UpdatePlayer(player);
}

int close(StreamPlayer *player)
{

    ClosePlayerFile(player);
    printf("Done.\n");

    /* All files done. Delete the player, and close down OpenAL */
    DeletePlayer(player);
    player = NULL;

    CloseAL();
    return 0;
}
