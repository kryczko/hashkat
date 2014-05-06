#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <uv.h>

#include "util.h"

#include "analyzer.h"

#include <luawrap-lib/include/luawrap/luawrap.h>

// Defined in interactive_mode.cpp:
lua_State* get_lua_state(AnalysisState& state);

static void lua_hook(AnalysisState& state, const char* type, int id1 = -1, int id2 = -1) {
    lua_State* L = get_lua_state(state);

    luawrap::globals(L)["time"] = state.time;

    lua_getglobal(L, type);
    lua_pushinteger(L, id1);
    lua_pushinteger(L, id2);
    lua_call(L, 2, 0);
}

void lua_hook_follow(AnalysisState& state, int id_follower, int id_followed) {
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_follow", id_follower, id_followed);
    }
}

void lua_hook_add(AnalysisState& state, int id) {
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_add", id);
    }
}

void lua_hook_unfollow(AnalysisState& state, int id_follower, int id_followed) {
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_unfollow", id_follower, id_followed);
    }
}

void lua_hook_tweet(AnalysisState& state, int id_tweeter, int id_tweet) {
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_tweet", id_tweeter, id_tweet);
    }
}

void lua_hook_exit(AnalysisState& state) {
    if (state.config.enable_lua_hooks) {
        lua_hook(state, "on_exit");
    }
}

