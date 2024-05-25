
#include "main.h"

#include <SDL2/SDL.h>
#include <physfs.h>
#include <stdio.h>

#include "font.h"
#include "game.h"
// #include "script.h"
#include "sound.h"
#include "video.h"

int main(int argc, char *argv[]) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, ENGINE_DISCLAIMER);

  if (!PHYSFS_init(NULL)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[main] fail to initialize FS loader %s\n",
                 PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return MAIN_ERROR_DATA;
  }
  PHYSFS_mount(argv[1], "/", 1);
  PHYSFS_mount("data", "/", 1);
  if (!PHYSFS_mount(DATA_PACKAGE_KENNEY, "/kenney/", 1)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[main] fail to load archive %s: %s \n", DATA_PACKAGE_KENNEY,
                 PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return MAIN_ERROR_DATA;
  }
  PHYSFS_mount("card_frames_rev.zip", "/", 1);
  

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[main] initialize sound/music...\n");
  sound_init();


  // load some sound to demo the engine
  int sd_num = -1;
  int sd_mus = -1;
  sd_num = sound_load("/explosion.wav");
  sd_mus = music_load("/scifi-fantasy-soundscape-21618.mp3");


  video_init();

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
              "[main] initializing script engine...");
  // struct ScriptState *script_state = init_script(SCRIPTING_DEFAULT_PATH);

  struct Image *im1 = load_image("/images/spritesheet_50x50_fixed.png");
  // SDL_Rect src1 = {0, 0 + 150, 50, 50};
  int vel_x = 0;
  int vel_y = 0;

  struct Image *im2 = load_image("/kenney/PNG/Outline/Default/hand_point.png");
  SDL_Texture *te2 = SDL_CreateTextureFromSurface(renderer, im2->surface);
  int te2_w; int te2_h;
  SDL_QueryTexture(te2, NULL, NULL, &te2_w, &te2_h);

  // SDL_Surface *surface;
  SDL_Event event;
  Uint64 previousTime = SDL_GetTicks64();
  Uint64 current = SDL_GetTicks64();
  Uint64 startTime = SDL_GetTicks64();
  int should_stop = 0;
  int nb_frame = 0;
  Uint64 dt = 0;
  int mouse_x = 0;
  int mouse_y = 0;
  while (!should_stop) {
    previousTime = current;
    current = SDL_GetTicks64();

    dt += current - previousTime;

    if (dt > 100) {
      nb_frame += 1;
      // nb_frame = nb_frame % 4;
      if (nb_frame >= 4) {
        nb_frame = 0;
      }
      vel_x += 2;
      dt = dt - 100;
    }
    // game_update(&game, current - previousTime);
    // SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "dt: %d", dt);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

    // const SDL_Rect srcrect = {5, 5};
    // const SDL_Rect distrect = {5, 5};
    // SDL_BlitSurface(window, &srcrect, im1->surface, distrect);

    SDL_RenderClear(renderer);

    SDL_Rect src1 = {50 + (nb_frame * 50), 0 + 150, 50, 50};
    SDL_Rect dst1 = {20 + vel_x, 20, 100, 100};
    SDL_RenderCopy(renderer, im1->texture, &src1, &dst1);

    //SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_Rect src2 = {0, 0, te2_w, te2_h};
    SDL_Rect dst2 = {mouse_x - 10, mouse_y - 10, te2_w, te2_h};
    SDL_RenderCopy(renderer, im2->texture, &src2, &dst2);


    SDL_RenderPresent(renderer);

    SDL_Delay(1);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_MOUSEMOTION: // update mouse coords
        mouse_x = event.motion.x;
        mouse_y = event.motion.y;
        break;
      // case SDL_KEYUP:
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          should_stop = 1;
          break;
        case SDLK_RIGHT:
          vel_x = vel_x + 3;
          SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Scancode: 0x%02X",
                      event.key.keysym.scancode);
          //SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "vel_x: %d", vel_x);

          //sound_start(sd_num);
          music_stop(sd_mus);

          break;
        case SDLK_LEFT:
          vel_x = vel_x - 3;
          SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Scancode: 0x%02X",
                      event.key.keysym.scancode);
          SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "vel_x: %d", vel_x);
          
          music_start(sd_mus);

          sound_stop(sd_num);
          break;
        }
        break;
      case SDL_QUIT:
        should_stop = 1;
        break;
      }
    }

    /*if ((current - startTime) > 2000)
    {
        printf("end of game\n");
        video_channel->should_stop = 1;
        break;
    }*/
  }

  // script_close(script_state);

  // free_image(im1);

  video_quit();

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[main] closing SDL...\n");
  SDL_Quit();

  sound_quit();

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[main] closing FS loader...\n");
  if (!PHYSFS_deinit()) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "fail to deinit PHYSFS %s\n",
                PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return MAIN_ERROR_DATA;
  }

  return 0;
}
