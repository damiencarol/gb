
#include "sound.h"

#ifdef WITH_SOUND_SDL

#include <physfs.h>
#include "physfsrwops.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>

//The music that will be played
Mix_Music* musics[MAX_MUSICS];
int current_music = -1;

//The sound effects that will be used
Mix_Chunk *sounds[MAX_SOUNDS];
int current_sound = -1;

int sound_init() {
  // init SDL
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[sound] Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }
  //Initialize SDL_mixer
  if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
  {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[sound] SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
      return -1;
  }
  return 0;
}

void sound_quit() {
  Mix_Quit();
}


int sound_load(char * name)
{
  if (!PHYSFS_exists(name)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[sound]FS loader can't load the file %s", name);
    return -1;
  }
  SDL_RWops *rw = PHYSFSRWOPS_openRead(name);
  if (rw == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "can't load the file %s", name);
    return -1;
  }
  int ret = ++current_sound;
  sounds[ret] = Mix_LoadWAV_RW(rw, 0);
  SDL_FreeRW(rw);
  return ret;
}

void sound_free() {
    //Mix_FreeChunk( sound );
}

void sound_start(int num){
  Mix_PlayChannel( -1, sounds[num], 0 );
}

void sound_stop(int num){
  //Mix_StopChannel(-1, sound);
}

/**
 * Load music
 */
int music_load(char *name)
{
  if (!PHYSFS_exists(name)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[sound] FS loader can't load the file %s", name);
    return -1;
  }
  SDL_RWops *rw = PHYSFSRWOPS_openRead(name);
  if (rw == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[sound] can't load the file %s", name);
    return -1;
  }
  int ret = ++current_music;
  musics[ret] = Mix_LoadMUS_RW(rw, 0);
  if( musics[ret] == NULL )
  {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
      return -1;
  }
  //SDL_FreeRW(rw);
  return ret;
}

void music_start(int num){
  Mix_PlayMusic( musics[num], 0 );
}

void music_stop(int num){
  Mix_PauseMusic( );
}

void music_set_volume(int num, int new_vol){
  Mix_MasterVolume(new_vol);
}

Mix_MusicType get_internal_music(int num){
  return Mix_GetMusicType(musics[num]);
}

#endif