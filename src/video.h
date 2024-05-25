
#pragma once

#include <SDL2/SDL.h>

#define SDL_MAX_FPS 60
#define WINDOWS_WIDTH 640
#define WINDOWS_HEIGH 480

extern SDL_Window *window;
extern SDL_Renderer *renderer;

struct Image
{
    SDL_Surface *surface;
    SDL_Texture *texture;
};

int video_init();
int video_quit();

struct Image *load_image(const char *name);
