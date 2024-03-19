
#pragma once

#include <physfs.h>
#include <lua.h>
#include <lauxlib.h>

struct ScriptState
{
    lua_State *L;
};

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize);
struct ScriptState *init_script(const char *main_script_path);
void script_close(struct ScriptState *state);
