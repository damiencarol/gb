#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <physfs.h>

#include "physfsrwops.h"
#include "video.h"

struct Image *load_image(const char *name) {
  if (!PHYSFS_exists(name)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "FS loader can't load the file %s", name);
    return NULL; // file doesn't exist
  }
  struct Image *image;
  image = malloc(1 * sizeof(struct Image));
  SDL_RWops *rw = PHYSFSRWOPS_openRead(name);
  if (rw == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "can't load the file %s", name);
    return NULL; // file doesn't exist
  }
  image->surface = IMG_Load_RW(rw, 0);
  SDL_FreeRW(rw);
  return image;
}

void free_image(struct Image *image) {
  SDL_FreeSurface(image->surface);
  free(image);
}
