#ifndef TEAMOR8BIT_PLAYER_HPP
#define TEAMOR8BIT_PLAYER_HPP

#include<glm/glm.hpp>

namespace gamespace {
    class Player {
    private:
        glm::vec3 _color;
    public:
        Player(glm::vec3 c): _color(c) {}
        Player(const Player &o) = delete;
        Player(Player &&o) = delete;
        Player &operator=(const Player &o) = delete;
        Player &operator=(Player &&o) = delete;
        glm::vec3 color() {
            return _color;
        }
    };
}

#endif //TEAMOR8BIT_PLAYER_HPP
