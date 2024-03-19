
#pragma once

#include "ship.h"
#include "player.h"

struct Game
{
    struct Ship *ships;
    struct Player *players;
};

void game_update(struct Game * game, Uint64 ticks);
