#include "scripting.hpp"

using namespace gamespace;

unit_id Scripting::next_id;

Scripting::Scripting(GameBoard *board): _map(board), _state(lua_open(), &lua_close) {
    auto *state = _state.get();
    luaL_openlibs(state);
    lua_pushlightuserdata(state, _map);
    lua_setglobal(state, "_map");
    lua_getglobal(state, "package");
    lua_pushstring(state, "path");
    lua_pushstring(state, "./?.lua");
    lua_settable(state, -3);
    // load loader script
    luaL_dofile(state, "static/loader.lua");
}

void Scripting::loadScript(std::string path) {
    auto *state = _state.get();
    // push path
    lua_getglobal(state, "load_script");
    lua_pushstring(state, path.c_str());
    // call load_script
    switch(lua_pcall(state, 1, 2, 0)) {
        case LUA_ERRRUN: throw lua_runtime_error(lua_tostring(state, -1), __FILE__, __LINE__);
        case LUA_ERRMEM: throw lua_runtime_error(lua_tostring(state, -1), __FILE__, __LINE__);
    }
    // get info via return values
    assert(lua_isstring(state, -2));
    assert(lua_isstring(state, -1));
    // load mesh
    struct unit_info info;
    info.name = lua_tostring(state, -2);
    unit_id id = info.id = next_id++;
    info.path = lua_tostring(state, -1);
    info.mesh.reset(new qe::Mesh<qe::OBJV1>(qe::Loader<qe::OBJV1>(info.path)));
    // add to _registered_units
    _registered_units[id] = std::move(info);
    // load module into lua _units
    lua_getglobal(state, "_units");
    lua_pushstring(state, _registered_units[id].name.c_str());
    lua_getglobal(state, "tmp_module");
    lua_settable(state, -3);
}
