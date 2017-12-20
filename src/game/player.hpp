#ifndef TEAMOR8BIT_PLAYER_HPP
#define TEAMOR8BIT_PLAYER_HPP

#include<glm/glm.hpp>

namespace gamespace {
    class Player {
    private:
        glm::vec3 _color;
    public:
        Player(glm::vec3 c): _color(c) {}
        bool operator==(const Player &o) {return _color == o._color;}
        bool operator!=(const Player &o) {return !(*this == o);}
        glm::vec3 color() {
            return _color;
        }
    };
}

#endif //TEAMOR8BIT_PLAYER_HPP
