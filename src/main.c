

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

    struct ScriptState *script_state = init_script(SCRIPTING_DEFAULT_PATH);

    struct VideoMainChannel *video_channel = video_new_channel();
    SDL_Thread *graphic_thread = SDL_CreateThread(rendering_thread, "graphic_thread", video_channel);

    // SDL_Surface *surface;
    SDL_Event event;
    Uint64 current = SDL_GetTicks64();
    Uint64 startTime = SDL_GetTicks64();
    while (1)
    {
        current = SDL_GetTicks64();
        // game_update(&game, current - previousTime);

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            //printf("video_channel->should_stop = %d\n", video_channel->should_stop);
            video_channel->should_stop = 1; // stop video engine
            break;
        }

        //SDL_Delay(10);

        /*if ((current - startTime) > 2000)
        {
            printf("end of game\n");
            video_channel->should_stop = 1;
            break;
        }*/
    }

    int status_graphic_thread;
    SDL_WaitThread(graphic_thread, &status_graphic_thread);
    printf("[Video] closing thread\n");
    SDL_Quit();

    script_close(script_state);

    if (!PHYSFS_deinit())
    {
        printf("fail to deinit PHYSFS %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return MAIN_ERROR_DATA;
    }

    return 0;
}
