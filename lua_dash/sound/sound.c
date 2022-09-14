#include "alhelpers.h"
#include "openal.h"

//TODO define this in lua, and create this in C from reading that.
/**
 * @brief Structure to hold a bgm with it's loop points.
 */
typedef struct Sg_Bgm {
    char* bgm_name;
    double loop_begin;
    double loop_end;

} Sg_Bgm;

typedef struct Sg_Sfx {
    char* sfx_name;
    Sg_Loaded_Sfx* loaded_sfx;
} Sg_Sfx;

static Sg_Bgm bgm_music[]= {
    {"build/assets/victory2.ogg", 28.220, 51.255}
};

static Sg_Sfx sfx_sounds[] =
{
    {"build/assets/jump.ogg", NULL}
};


int InitializeSound()
{
    return InitializeAl();
}

int PlayBgm(int bgm_number)
{

    return PlayBgmAl(bgm_music[bgm_number].bgm_name, &bgm_music[bgm_number].loop_begin,&bgm_music[bgm_number].loop_end);

}

int PlaySfxOneShot(int sfx_number)
{
    if (!sfx_sounds[sfx_number].loaded_sfx)
    {
        sfx_sounds[sfx_number].loaded_sfx = LoadSfxFileAl(sfx_sounds[sfx_number].sfx_name);
    }

    PlaySfxAl(sfx_sounds[sfx_number].loaded_sfx);
    return 1;

}
void UpdateSound()
{
    UpdateAl();
}

void CloseSound()
{
    CloseAL();

}
