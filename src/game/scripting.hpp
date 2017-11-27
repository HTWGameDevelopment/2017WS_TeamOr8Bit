#ifndef __GAME__SCRIPTING_HPP
#define __GAME__SCRIPTING_HPP

#include<game/board.hpp>

#include<engine/mesh.hpp>

extern "C" {
#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>
}

#include<map>
#include<stdexcept>

namespace gamespace {

    typedef int unit_id;

    class Scripting {
    private:
        static unit_id next_id;
        GameBoard *_map;
        std::unique_ptr<lua_State, decltype(&lua_close)> _state;
        struct unit_info {
            std::string name;
            unit_id id;
            std::string path;
            std::unique_ptr<qe::Mesh<qe::OBJV1>> mesh;
        };
        std::map<unit_id, unit_info> _registered_units;
    public:
        Scripting(GameBoard *board);
        void loadScript(std::string path);
    };

    struct lua_runtime_error: public std::runtime_error {
        lua_runtime_error(std::string s, const char *file, int line): runtime_error(std::string(file) + ":" + std::to_string(line) + ": Lua runtime error\n" + s) {}
    };

}

#endif
