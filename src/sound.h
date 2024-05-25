
#define WITH_SOUND_SDL 1

#ifdef WITH_SOUND_SDL
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#else
#include <AL/al.h>
#include <AL/alc.h>

// OpenAL internals
#define NUM_BUFFERS 20
#endif

#define MAX_MUSICS 20
#define MAX_SOUNDS 100


int sound_init();
void sound_quit();
int sound_load(char *);
void sound_start(int num);
void sound_stop(int num);

void music_start(int num);
void music_stop(int num);
void music_set_volume(int num, int new_vol);

int music_load(char *);

#ifdef OPENAL
static void list_audio_devices(const ALCchar *devices);
#else
Mix_MusicType get_internal_music(int num);
#endif
