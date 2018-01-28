#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include<game/terrain.hpp>

#include<hextile/hextile.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/transform.hpp>
#include<glm/mat4x4.hpp>
#include<glm/vec2.hpp>
#include<stdio.h>

namespace gamespace {

    class Unit;
    class GameBoard;

    typedef hextile::hexpoint_t hexpoint_t;
    typedef glm::mat4 render_data_t;

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
        std::array<hextile::marker_t, 4> _marker_layer;
        render_data_t _render_offsets;
        Terrain _terrain;
    public:
        static constexpr float dim_x = 2 * 0.866; // dimension in X direction
        static constexpr float dim_y = 2 * 1.0; // dimension in Y direction
        BoardTile(GameBoard *t, hextile::hexpoint_t o): _unit(nullptr), _board(t), _p(o), _render_offsets(glm::translate(glm::vec3(0,0,0))), _terrain(Terrain::getNormalTile()) {}
        BoardTile(const BoardTile &other) = delete;
        BoardTile(BoardTile &&other): _unit(other._unit), _board(other._board), _p(other._p), _marker_layer(other._marker_layer), _render_offsets(other._render_offsets), _terrain(other._terrain) {
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
        void setUnit(Unit *unit);
        void clearUnit() {
            _unit = nullptr;
        }
        Unit *unit() {
            return _unit;
        }
        hextile::hexpoint_t coord() {
            return _p;
        }
        Terrain &terrain() {
            return _terrain;
        }
        GameBoard &board();
        bool mark(unsigned int layer, unsigned int d);
        bool marked(unsigned int layer);
        unsigned int marked_value(unsigned int layer);
        unsigned int lastMarkerId(unsigned int layer) {
            return _marker_layer[layer].id;
        }
        unsigned int marker(unsigned int layer) {
            if(marked(layer)) return _marker_layer[layer].val;
            return 0;
        }
        void destroyUnit();
        render_data_t &renderData() {
            return _render_offsets;
        }
        void __introspect(size_t off);
    };

}

#endif
