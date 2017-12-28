#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include<hextile/hextile.hpp>

#include<glm/vec2.hpp>
#include<stdio.h>

namespace gamespace {

    class Unit;
    class GameBoard;

    typedef hextile::hexpoint_t hexpoint_t;

    const unsigned int VISIBILITY_LAYER = 0;
    const unsigned int ACTION_LAYER = 1;
    const unsigned int MOVE_LAYER = 2;
    const unsigned int AOE_LAYER = 3;

    /**
     * \brief Our tile type for HexTile elements
     */
    class BoardTile {
    private:
        Unit *_unit;
        GameBoard *_board; // pointer to board
        hextile::hexpoint_t _p; // coordinate
        std::array<hextile::marker_t, 3> _marker_layer;
    public:
        static constexpr float dim_x = 2 * 0.866; // dimension in X direction
        static constexpr float dim_y = 2 * 1.0; // dimension in Y direction
        BoardTile(GameBoard *t, hextile::hexpoint_t o): _unit(nullptr), _board(t), _p(o) {}
        BoardTile(const BoardTile &other) = delete;
        BoardTile(BoardTile &&other): _unit(other._unit), _board(other._board), _p(other._p), _marker_layer(other._marker_layer) {
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
        bool mark(unsigned int layer, unsigned int d);
        bool marked(unsigned int layer);
        unsigned int lastMarkerId(unsigned int layer) {
            return _marker_layer[layer].id;
        }
        unsigned int marker(unsigned int layer) {
            if(marked(layer)) return _marker_layer[layer].val;
            return 0;
        }
        void destroyUnit();
    };

}

#endif
