#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include<hextile/hextile.hpp>

#include<glm/vec2.hpp>
#include<stdio.h>

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
        hextile::marker_t _mark;
    public:
        static constexpr float dim_x = 2 * 0.866; // dimension in X direction
        static constexpr float dim_y = 2 * 1.0; // dimension in Y direction
        ~BoardTile();
        void board(hextile::HexTile<BoardTile> *t) {
            _board = t;
            _mark.id = 0;
            _mark.val = 0;
        }
        BoardTile &operator=(hextile::hexpoint_t o) {
            _p = o;
            return *this;
        }
        glm::vec2 centerPos() {
            glm::vec2 r(dim_x * _p.x, 0.75 * dim_y * _p.y);
            if(_p.y % 2 == 1) {
                r.x -= 0.5 * dim_x;
            }
            return r;
            // if((_p.x + 1) % 2 == 1)
            //     return glm::vec2(2 * dim_x * (_p.x / 2), 3 * dim_y * (_p.y + 1));
            // else
            //     return glm::vec2(2 * dim_x * (_p.x / 2) - dim_x, 3 * dim_y * (_p.y + 1) + 1.5 * dim_y);
        }
        void setUnit(Unit *unit) {
            _unit = unit;
        }
        Unit *unit() {
            return _unit;
        }
        hextile::hexpoint_t coord() {
            return _p;
        }
        hextile::HexTile<BoardTile> &board() {
            return *_board;
        }
        bool mark(unsigned int d);
        bool marked() {
            return _mark.id == _board->currentMarker() && _mark.val != 0;
        }
        unsigned int lastMarkerId() {
            return _mark.id;
        }
        unsigned int marker() {
            if(marked()) return _mark.val;
            return 0;
        }
        void destroyUnit();
    };

    typedef hextile::hexpoint_t hexpoint_t;

    class GameBoard: public hextile::HexTile<BoardTile> {
    public:
        GameBoard(int x, int y): hextile::HexTile<BoardTile>(x, y) {}
        void moveUnit(hexpoint_t from, hexpoint_t to);
        void attackUnit(hexpoint_t from, hexpoint_t to);
    };

}

#endif
