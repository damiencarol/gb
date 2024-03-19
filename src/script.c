
#include <stdlib.h>
#include <stdio.h>

#include "script.h"

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    (void)ud;
    if (nsize == 0)
    {
        //printf("FREE: %lu\n", osize);
        free(ptr);
        return NULL;
    }
    else
    {
        //printf("ALLOC: %lu %lu\n", osize, nsize);
        return realloc(ptr, nsize);
    }
}

int load_font(lua_State* L)
{
    const char *a = luaL_checkstring(L, 1);
    // TODO: load font
    lua_pushinteger(L, 0);
    return 1;
}

int load_music(lua_State* L)
{
    const char *a = luaL_checkstring(L, 1);
    // TODO: load music
    lua_pushinteger(L, 0);
    return 1;
}

struct ScriptState* init_script(const char *main_script_path)
{
       // Lua context
    lua_State *L = lua_newstate(l_alloc, NULL);
    // define API
    const struct luaL_Reg script_api[] = {
        { "load_font", load_font },
        { "load_music", load_music }
    };
    // We create a new table
    lua_newtable(L);
    // Here we set all functions from api array into
    // the table on the top of the stack
    luaL_setfuncs(L, &script_api, 0);
    // We get the table and set as global variable
    lua_setglobal(L, "api");

    // if the scripting part exists, load it and run it
    if (PHYSFS_exists(main_script_path)) 
    {
        PHYSFS_file* main_script = PHYSFS_openRead(main_script_path);
        PHYSFS_sint64 file_size_main_script = PHYSFS_fileLength(main_script);
        char * code_main_script;
        code_main_script = malloc(file_size_main_script + 1);
        code_main_script[file_size_main_script] = 0;
        PHYSFS_sint64 nb_read = PHYSFS_readBytes(main_script, code_main_script, file_size_main_script);
        if (nb_read != file_size_main_script) {

            printf("[Scripting] fail to read main script: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        }
        // Here we load the string and use lua_pcall for run the code
        int lua_err = luaL_dostring(L, code_main_script);
        if (lua_err != LUA_OK) {
            printf("fail to run the main script\n");
        }


        // test
        int lua_err2 = luaL_dostring(L, "load()");
        if (lua_err2 != LUA_OK) {
            printf("[Scripting] fail to run the main script function 'load' \n");
            luaL_traceback(L, L, NULL, 1);
            printf("%s\n", lua_tostring(L, -1));
        }
    } else {
        printf("[Scripting] fail to found main script\n");
    }

};

void script_close(struct ScriptState *state)
{
    lua_close(state->L);
}
