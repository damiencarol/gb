#include <physfs.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vulkan/vulkan.h>

#include "physfsrwops.h"
#include "video.h"

struct Image *load_image(const char *name)
{
    struct Image *image;
    image = malloc(1 * sizeof(struct Image));
    SDL_RWops *rw = PHYSFSRWOPS_openRead(name);
    if (rw == NULL)
    {
        return NULL; // file doesn't exist
    }
    image->surface = IMG_Load_RW(rw, 0);
    SDL_FreeRW(rw);
    return image;
}

struct VideoMainChannel *video_new_channel()
{
    struct VideoMainChannel *video_channel = malloc(sizeof(video_channel));
    video_channel->mutex = SDL_CreateMutex();
    video_channel->should_stop = 0;
    if (!video_channel->mutex)
    {
        printf("%s\n", SDL_GetError());
    }
    return video_channel;
}

int rendering_thread(void *data)
{
    // struct Image* im1 = load_image("data/images/spritesheet_50x50_fixed.png");

    struct VideoMainChannel *video_channel = (struct VideoMainChannel *)data;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(WINDOWS_WIDTH, WINDOWS_HEIGH, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    // Uint64 previousTime = SDL_GetTicks64();
    // Uint64 current = SDL_GetTicks64();
    int running = 1;
    while (running)
    {
        // current = SDL_GetTicks64();

        // draw here
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

        /*SDL_Rect srcrect = {5, 5};
        SDL_Rect distrect = {5, 5};
        SDL_BlitSurface(im1, srcrect, window, distrect);*/

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        // frame limiter
        // SDL_Delay(1000 / SDL_MAX_FPS - SDL_GetTicks64() + previousTime);
        // printf("before delay\n");
        SDL_Delay(1);
        // printf("after delay\n");
        // previousTime = current;
        // SDL_LockMutex(video_channel->mutex);
        if (video_channel->should_stop)
        {
            // printf("should stop\n");
            // printf("video_channel->should_stop = %d\n", video_channel->should_stop);
            running = 0;
        }
        // SDL_UnlockMutex(video_channel->mutex);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
