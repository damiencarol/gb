

#include <stdio.h>
#include <SDL2/SDL.h>
#include <physfs.h>
#include <lua.h>
#include <lauxlib.h>

#include "main.h"
#include "game.h"
#include "video.h"
#include "font.h"
#include "script.h"

int main(int argc, char *argv[])
{

    printf(ENGINE_DISCLAIMER);
    printf("\n");

    if (!PHYSFS_init(NULL))
    {
        printf("fail to initialize PHYSFS %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return MAIN_ERROR_DATA;
    }
    PHYSFS_mount("data", "/", 1);

    // init SDL
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


    struct ScriptState *script_state = init_script(SCRIPTING_DEFAULT_PATH);




    struct Image* im1 = load_image("data/images/spritesheet_50x50_fixed.png");
    SDL_Texture* te1 = SDL_CreateTextureFromSurface(renderer, im1->surface);
    SDL_Rect src1 = {0, 0, 30, 30};
    SDL_Rect dst1 = { 50, 50, 80, 80 };


    // SDL_Surface *surface;
    SDL_Event event;
    Uint64 current = SDL_GetTicks64();
    Uint64 startTime = SDL_GetTicks64();
    int should_stop = 0;
    while (!should_stop)
    {
        current = SDL_GetTicks64();
        // game_update(&game, current - previousTime);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

        //const SDL_Rect srcrect = {5, 5};
        //const SDL_Rect distrect = {5, 5};
        //SDL_BlitSurface(window, &srcrect, im1->surface, distrect);

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, te1, &src1, &dst1); // Affiche ma texture sur touts l'écran

        SDL_RenderPresent(renderer);
    


        SDL_PollEvent(&event);
        switch ( event.type )
        {
            case SDL_QUIT:
            //case SDL_KEYDOWN:
                //printf("video_channel->should_stop = %d\n", video_channel->should_stop);
                //video_channel->should_stop = 1; // stop video engine
                should_stop = 1;
                break;
        }


        //SDL_Delay(1);

        /*if ((current - startTime) > 2000)
        {
            printf("end of game\n");
            video_channel->should_stop = 1;
            break;
        }*/
    }

    script_close(script_state);


    //free_image(im1);

 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);



    printf("[PhysFS] closing...\n");
    if (!PHYSFS_deinit())
    {
        printf("fail to deinit PHYSFS %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return MAIN_ERROR_DATA;
    }

    SDL_Quit();

    return 0;
}
