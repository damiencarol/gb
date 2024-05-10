
#include "sound.h"
#include <AL/al.h>
#include <stdio.h>

void test() {
  ALenum err = alGetError();
  printf("%s\n", alGetString(err));
}

void load_music() {}
