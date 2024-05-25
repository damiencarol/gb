
#if defined(ENABLE_SCRIPT)

#include <SDL2/SDL.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include "script.h"

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
  if (nsize == 0) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[script] FREE: %lu\n", osize);
    free(ptr);
    return NULL;
  } else {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[script] ALLOC: %lu %lu\n",
                osize, nsize);
    // return realloc(ptr, nsize);
    return realloc(ptr, nsize);
  }
}

static int load_font(lua_State *L) {
  const char *a = luaL_checkstring(L, 1);
  // TODO: load font
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[script] load_font with %s\n", a);
  lua_pushinteger(L, 0);
  return 1;
}

static int load_music(lua_State *L) {
  const char *a = luaL_checkstring(L, 1);
  // TODO: load music
  printf("load_music with %s\n", a);
  lua_pushinteger(L, 0);
  return 1;
}

int api_load_image(lua_State *L) {
  const char *a = luaL_checkstring(L, 1);
  // TODO: load music
  printf("load_image with %s\n", a);
  lua_pushinteger(L, 0);
  return 1;
}

struct ScriptState *init_script(const char *main_script_path) {

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "0\n");
  lua_State *L = lua_newstate(l_alloc, NULL);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "0a\n");
  luaL_openlibs(L);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "0b\n");
  lua_register(L, "load_image", api_load_image);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "1\n");
  // if the scripting part exists, load it and run it
  if (PHYSFS_exists(main_script_path)) {
    PHYSFS_file *main_script = PHYSFS_openRead(main_script_path);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "2\n");
    PHYSFS_sint64 file_size_main_script = PHYSFS_fileLength(main_script);
    char *code_main_script;
    code_main_script = (char *)malloc(file_size_main_script + 1);
    code_main_script[file_size_main_script] = 0;
    PHYSFS_sint64 nb_read =
        PHYSFS_readBytes(main_script, code_main_script, file_size_main_script);
    if (nb_read != file_size_main_script) {

      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                  "[Scripting] fail to read main script: %s\n",
                  PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
    // Here we load the string and use lua_pcall for run the code
    int lua_err = luaL_dostring(L, code_main_script);
    if (lua_err != LUA_OK) {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                  "fail to run the main script\n");
    } else {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "main script finished\n");
    }

    // test
    int lua_err2 = luaL_dostring(L, "load()");
    if (lua_err2 != LUA_OK) {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                  "[Scripting] fail to run the main script function 'load' \n");
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s\n", lua_tostring(L, -1));
    }
  } else {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "[Scripting] fail to found main script\n");
  }
  struct ScriptState *ret =
      (struct ScriptState *)malloc(1 * sizeof(struct ScriptState));
  ret->L = L;
  return ret;
};

void script_close(struct ScriptState *state) { lua_close(state->L); }

#endif
