#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include<hextile/hextile.hpp>

#include<glm/vec2.hpp>

namespace gamespace {

    class BoardTile {
    private:
        hextile::hexpoint_t _p;
        const float dim_x = 0.866;
    public:
        BoardTile &operator=(hextile::hexpoint_t o) {
            _p = o;
            return *this;
        }
        glm::vec2 centerPos() {
            if((_p.x + 1) % 2 == 1)
                return glm::vec2(2 * dim_x * (_p.x / 2), 3 * (_p.y + 1));
            else
                return glm::vec2(2 * dim_x * (_p.x / 2) - dim_x, 3 * (_p.y + 1) + 1.5);
        }
    };

    typedef hextile::HexTile<BoardTile> GameBoard;

}

#endif
