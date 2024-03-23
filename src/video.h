
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

struct Image
{
    SDL_Surface *surface;
};

int rendering_thread(void *data);
struct VideoMainChannel *video_new_channel();
struct Image *load_image(const char *name);
