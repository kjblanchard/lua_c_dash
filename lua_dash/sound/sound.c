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

static Sg_Bgm bgm_music[]= {
    {"build/assets/victory2.ogg", 28.220, 51.255}
};

int InitializeSound()
{
    return InitializeAl();
}

int PlayBgm(int bgm_number)
{

    return PlayBgmAl(bgm_music[bgm_number].bgm_name, &bgm_music[bgm_number].loop_begin,&bgm_music[bgm_number].loop_end);

}

void UpdateSound()
{
    UpdateAl();
}

void CloseSound()
{
    CloseAL();

}
