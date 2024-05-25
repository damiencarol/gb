
#ifdef OPENAL_ENABLE

#include "sound.h"
#include <SDL2/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <stdio.h>
#include <string.h>

ALCdevice *device;
ALCcontext *context;
ALuint buffers[NUM_BUFFERS];
int current_buffer_num = -1;

void sound_init() {
  const ALchar *deviceName = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[sound] device detected: %s ",
              deviceName);
  // list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

  ALboolean enumeration;
  ALenum error_main;
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[sound] alcOpenDevice...");
  device = alcOpenDevice(deviceName);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[sound] alcOpenDevice. DONE");
  // SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[sound] alcOpenDevice. DONE",
  // device->);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[sound] alcOpenDevice. DONE");
  // if (error_main != AL_NO_ERROR) {
  if (!device) {
    error_main = alGetError();
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "[sound] alcOpenDevice error: %x", error_main);
    return;
  }

  if (device) {
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
  }
  alGenBuffers(NUM_BUFFERS, buffers);
  ALenum error = alcGetError(device);
  if (error != AL_NO_ERROR) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[sound] alGenBuffers error: %d",
                 error);
    return;
  }
}

void load_music() {}

void sound_quit() {
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

static void list_audio_devices(const ALCchar *devices) {
  const ALCchar *device = devices, *next = devices + 1;
  size_t len = 0;

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Devices list:\n");
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "----------\n");
  while (device && *device != '\0' && next && *next != '\0') {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s\n", device);
    len = strlen(device);
    device += (len + 1);
    next += (len + 2);
  }
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "----------\n");
}


int sound_load(char * file)
{
  ALCenum error;

  ALsizei size, freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop = AL_FALSE;

  alutCreateBufferFromFile();
  alutLoadWAVFile(file, &format, &data, &size, &freq, &loop);
  error = alGetError();
  if (error != AL_NO_ERROR) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[sound] alGenBuffers error: %d",
                 error);
    return -1;
  }

  int i = current_buffer_num++;

  return i;
}

void sound_start(int num){}
void sound_stop(int num){}

#endif