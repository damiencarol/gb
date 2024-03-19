
#pragma once

#include <SDL2/SDL.h>

#include "player.h"

struct Ship
{
    Uint32 x;
    Uint32 y;
    struct Player *player;
};
