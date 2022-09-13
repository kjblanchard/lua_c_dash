#include "alhelpers.h"
#include "openal.h"

int InitializeSound()
{
    return InitializeAl();
}

int PlayBgm(int bgm_number)
{
    double loop_begin = 28.220;
    double loop_end = 51.255;

    return PlayBgmAl("build/assets/victory2.ogg",&loop_begin,&loop_end);

}

void UpdateSound()
{
    UpdateAl();
}

void CloseSound()
{
    CloseAL();

}
