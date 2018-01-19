#ifndef TEAMOR8BIT_PLAYER_HPP
#define TEAMOR8BIT_PLAYER_HPP

#include<glm/glm.hpp>

#include<logger.h>
#include<string>

namespace gamespace {
    class Player {
    private:
        glm::vec3 _color;
        std::string _name;
    public:
        Player(glm::vec3 c, std::string name): _color(c), _name(name) {}
        bool operator==(const Player &o) {return _color == o._color;}
        bool operator!=(const Player &o) {return !(*this == o);}
        glm::vec3 color() {
            return _color;
        }
        std::string name() {
            return _name;
        }
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "Player[" << _name << "," << GV3TOSTR(_color) << "]" << std::endl;
        }
    };
}

#endif //TEAMOR8BIT_PLAYER_HPP
