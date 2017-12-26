#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include<hextile/hextile.hpp>

#include<glm/vec2.hpp>
#include<stdio.h>

namespace gamespace {

    class Unit;
    class GameBoard;

    typedef hextile::hexpoint_t hexpoint_t;

    /**
     * \brief Our tile type for HexTile elements
     */
    class BoardTile {
    private:
        Unit *_unit;
        GameBoard *_board; // pointer to board
        hextile::hexpoint_t _p; // coordinate
        hextile::marker_t _mark;
    public:
        static constexpr float dim_x = 2 * 0.866; // dimension in X direction
        static constexpr float dim_y = 2 * 1.0; // dimension in Y direction
        BoardTile(GameBoard *t, hextile::hexpoint_t o): _unit(nullptr), _board(t), _p(o) {}
        BoardTile(const BoardTile &other) = delete;
        BoardTile(BoardTile &&other): _unit(other._unit), _board(other._board), _p(other._p), _mark(other._mark) {
            other._unit = nullptr;
            other._board = nullptr;
        }
        ~BoardTile();
        void setBoard(GameBoard *t) {
            _board = t;
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
        GameBoard &board();
        bool mark(unsigned int d);
        bool marked();
        unsigned int lastMarkerId() {
            return _mark.id;
        }
        unsigned int marker() {
            if(marked()) return _mark.val;
            return 0;
        }
        void destroyUnit();
    };

}

#endif
