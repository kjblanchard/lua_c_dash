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
    // Set the introloop so that we load two clips.
    assert(player != NULL);

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
    puts("Did I get here?");
    size_t frame_size;

    ClosePlayerFile(player);

    /* Open the audio file and check that it's usable. */
    int result = ov_fopen(filename, &player->vbfile);
    if (result !=0)
    {
        fprintf(stderr, "Could not open audio in %s: %d\n", filename, result);
        return 0;
    }
    player->vbinfo = ov_info(&player->vbfile, -1);
    if(!player->vbinfo)
    {
        puts("Vbinfo is null somehow");
    }

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
    puts("Got here");
  // data_len is the amount of data to read, allocate said data space
  // this is calculated by (samples * channels * 2 (aka 16bits))

    frame_size = (size_t)(BUFFER_SAMPLES * player->vbinfo->channels) * sizeof(short);
    player->membuf = malloc(frame_size);
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

/* Prebuffers some audio from the file, and starts playing the source */
static int StartPlayer(StreamPlayer *player)
{
    ALsizei i;

    /* Rewind the source position and clear the buffer queue */
    alSourceRewind(player->source);
    alSourcei(player->source, AL_BUFFER, 0);

    /* Fill the buffer queue */
    for (i = 0; i < NUM_BUFFERS; i++)
    {
        /* Get some data to give it to the buffer */

        int sel = 0;
        long slen = ov_read(&player->vbfile, (char*)player->membuf, BUFFER_SAMPLES, 0, sizeof(short), 1, 0);
        alBufferData(player->buffers[i], player->format, player->membuf, (ALsizei)slen,
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
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error starting playback\n");
        return 0;
    }

    return 1;
}
static int RestartStream(StreamPlayer *player, long slen, ALuint bufid)
{
    puts("Restarted loop");
    ov_time_seek_lap(&player->vbfile, 0.0);
    alBufferData(bufid, player->format, player->membuf, (ALsizei)slen,
                 player->vbinfo->rate);
    alSourceQueueBuffers(player->source, 1, &bufid);

    return 0;
}

static int UpdatePlayer(StreamPlayer *player)
{
    ALint processed, state, seconds;
    /* Get relevant source info */
    alGetSourcei(player->source, AL_SOURCE_STATE, &state);
    alGetSourcei(player->source, AL_BUFFERS_PROCESSED, &processed);
    alGetSourcei(player->source, AL_BUFFER, &seconds);
    if (alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error checking source state\n");
        return 0;
    }

    /* Unqueue and handle each processed buffer */
    while (processed > 0)
    {
        ALuint bufid;
        long slen;
        alSourceUnqueueBuffers(player->source, 1, &bufid);
        processed--;
        /* Read the next chunk of data, refill the buffer, and queue it
         * back on the source */
        slen = ov_read(&player->vbfile, (char*)player->membuf, BUFFER_SAMPLES, 0, sizeof(short), 1, 0);
        if (slen > 0)
        {
            alBufferData(bufid, player->format, player->membuf, (ALsizei)slen,
                         player->vbinfo->rate);
            alSourceQueueBuffers(player->source, 1, &bufid);
        }
        else
        {
            RestartStream(player, slen, bufid);
        }
        if (alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Error buffering data\n");
            return 0;
        }
    }

    /* Make sure the source hasn't underrun */
    if (state != AL_PLAYING && state != AL_PAUSED)
    {
        ALint queued;

        /* If no buffers are queued, playback is finished */
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

    /* Print out usage if no arguments were specified */

    if (InitAL(&filename2, &files_to_load_cound) != 0)
        return NULL; // Error

    player = NewPlayer();

    /* Play each file listed on the command line */
    for (i = 0; i < files_to_load_cound; i++)
    {
        const char *namepart;

        if (!OpenPlayerFile(player, filename))
            continue;

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
            continue;
        }

        return player;
    }

    return NULL;
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
