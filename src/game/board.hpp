#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include<hextile/hextile.hpp>

#include<glm/vec2.hpp>

namespace gamespace {

    class Unit;

    /**
     * \brief Our tile type for HexTile elements
     */
    class BoardTile {
    private:
        Unit *_unit;
        hextile::HexTile<BoardTile> *_board; // pointer to board
        hextile::hexpoint_t _p; // coordinate
        const float dim_x = 0.866; // dimension in X direction
        const float dim_y = 1.0; // dimension in Y direction
    public:
        void board(hextile::HexTile<BoardTile> *t) {
            _board = t;
        }
        BoardTile &operator=(hextile::hexpoint_t o) {
            _p = o;
            return *this;
        }
        glm::vec2 centerPos() {
            if((_p.x + 1) % 2 == 1)
                return glm::vec2(2 * dim_x * (_p.x / 2), 3 * dim_y * (_p.y + 1));
            else
                return glm::vec2(2 * dim_x * (_p.x / 2) - dim_x, 3 * dim_y * (_p.y + 1) + 1.5 * dim_y);
        }
        void setUnit(Unit *unit) {
            _unit = unit;
        }
        Unit *unit() {
            return _unit;
        }
    };

    typedef hextile::HexTile<BoardTile> GameBoard;
    typedef hextile::hexpoint_t hexpoint_t;

}

#include<game/unit.hpp>

#endif
