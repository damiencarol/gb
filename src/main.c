

#include <SDL2/SDL.h>
#include <lauxlib.h>
#include <lua.h>
#include <physfs.h>
#include <stdio.h>

#include "font.h"
#include "game.h"
#include "main.h"
#include "script.h"
#include "video.h"

int main(int argc, char *argv[]) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, ENGINE_DISCLAIMER);

  if (!PHYSFS_init(NULL)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "fail to initialize FS loader %s\n",
                 PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return MAIN_ERROR_DATA;
  }
  PHYSFS_mount(argv[1], "/", 1);

  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return 3;
  }
  SDL_Window *window;
  SDL_Renderer *renderer;
  if (SDL_CreateWindowAndRenderer(WINDOWS_WIDTH, WINDOWS_HEIGH,
                                  SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't create window and renderer: %s", SDL_GetError());
    SDL_Quit();
    return 3;
  }

  struct ScriptState *script_state = init_script(SCRIPTING_DEFAULT_PATH);

  struct Image *im1 = load_image("/images/spritesheet_50x50_fixed.png");
  SDL_Texture *te1 = SDL_CreateTextureFromSurface(renderer, im1->surface);
  //SDL_Rect src1 = {0, 0 + 150, 50, 50};
  SDL_Rect dst1 = {20, 20, 100, 100};
  int vel_x = 0;
  int vel_y = 0;

  // SDL_Surface *surface;
  SDL_Event event;
  Uint64 previousTime = SDL_GetTicks64();
  Uint64 current = SDL_GetTicks64();
  Uint64 startTime = SDL_GetTicks64();
  int should_stop = 0;
  Uint64 nb_frame = 0;
  while (!should_stop) {
    previousTime = current;
    current = SDL_GetTicks64();
    
    Uint64 dt = current - previousTime;
    
    // game_update(&game, current - previousTime);
    //SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "dt: %d", dt);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

    // const SDL_Rect srcrect = {5, 5};
    // const SDL_Rect distrect = {5, 5};
    // SDL_BlitSurface(window, &srcrect, im1->surface, distrect);

    SDL_RenderClear(renderer);

    SDL_Rect src1 = {50 + (nb_frame*50), 0 + 150, 50, 50};
    SDL_RenderCopy(renderer, te1, &src1, &dst1);

    SDL_RenderPresent(renderer);

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      //case SDL_KEYUP:
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          should_stop = 1;
          break;
        case SDLK_RIGHT:
          vel_x = vel_x + 3;
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Scancode: 0x%02X",
                    event.key.keysym.scancode);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "vel_x: %d",
                    vel_x);
          break;
        case SDLK_LEFT:
          vel_x = vel_x - 3;
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Scancode: 0x%02X",
                    event.key.keysym.scancode);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "vel_x: %d",
                    vel_x);
          break;
        }
        break;
      case SDL_QUIT:
        should_stop = 1;
        break;
      }
    }

    SDL_Delay(1);

    /*if ((current - startTime) > 2000)
    {
        printf("end of game\n");
        video_channel->should_stop = 1;
        break;
    }*/
  }

  script_close(script_state);

  // free_image(im1);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  printf("[main] closing FS loader...\n");
  if (!PHYSFS_deinit()) {
    printf("fail to deinit PHYSFS %s\n",
           PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return MAIN_ERROR_DATA;
  }

  SDL_Quit();

  return 0;
}
