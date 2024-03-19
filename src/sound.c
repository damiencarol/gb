
#include <stdio.h>
#include <AL/al.h>
#include "sound.h"

void test()
{
    ALenum err = alGetError();
    printf("%s\n", alGetString(err));
}

void load_music()
{

}
