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
        const float dim_x = 2 * 0.866; // dimension in X direction
        const float dim_y = 2 * 1.0; // dimension in Y direction
    public:
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
        bool mark(unsigned int d) {
            if(_board->currentMarker() == _mark.id) {
                if(_mark.val < d) {
                    _mark.val = d;
                    return true;
                } else return false;
            } else {
                _mark.id = _board->currentMarker();
                _mark.val = d;
                return true;
            }
        }
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
    };

    typedef hextile::hexpoint_t hexpoint_t;

    class GameBoard: public hextile::HexTile<BoardTile> {
    public:
        GameBoard(int x, int y): hextile::HexTile<BoardTile>(x, y) {}
        void moveUnit(hexpoint_t from, hexpoint_t to) {
            auto &ft = get(from);
            auto &tt = get(to);
            assert(ft.unit());
            assert(tt.unit() == nullptr);
            tt.setUnit(ft.unit());
            ft.setUnit(nullptr);
        }
    };

}

#include<game/unit.hpp>

#endif
