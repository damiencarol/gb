
#pragma once

#include <SDL2/SDL.h>

#define SDL_MAX_FPS 60
#define WINDOWS_WIDTH 640
#define WINDOWS_HEIGH 480

struct VideoMainChannel
{
    int is_initialized;
    int should_stop;
    SDL_mutex *mutex;
};

int rendering_thread(void *data);
struct VideoMainChannel *video_new_channel();
