#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <physfs.h>

#include "physfsrwops.h"
#include "video.h"

SDL_Window *window;
SDL_Renderer *renderer;

int video_init()
{
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[main] initializing video...");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[main] Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[main] initializing renderer...");

  if (SDL_CreateWindowAndRenderer(WINDOWS_WIDTH, WINDOWS_HEIGH,
                                  SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[main] Couldn't create window and renderer: %s",
                 SDL_GetError());
    SDL_Quit();
    return 3;
  }

  SDL_ShowCursor(0);

  return 0;
}

int video_quit()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}

struct Image *load_image(const char *name) {
  if (!PHYSFS_exists(name)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "FS loader can't load the file %s", name);
    return NULL; // file doesn't exist
  }
  struct Image *image;
  image = (struct Image *)malloc(1 * sizeof(struct Image));
  SDL_RWops *rw = PHYSFSRWOPS_openRead(name);
  if (rw == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "can't load the file %s", name);
    return NULL; // file doesn't exist
  }
  image->surface = IMG_Load_RW(rw, 0);
  SDL_FreeRW(rw);

  // build texture
  image->texture = SDL_CreateTextureFromSurface(renderer, image->surface);
  return image;
}

void free_image(struct Image *image) {
  SDL_FreeSurface(image->surface);
  free(image);
}
