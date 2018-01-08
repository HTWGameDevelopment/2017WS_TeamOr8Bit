#ifndef GAMESPACE_BOARD_HPP
#define GAMESPACE_BOARD_HPP

#include<game/board.hpp>

#include<hextile/hextile.hpp>

namespace gamespace {
    class GameBoard: public hextile::HexTile<BoardTile, 4> {
    public:
        GameBoard(int x, int y);
        GameBoard(const GameBoard &other) = delete;
        GameBoard(GameBoard &&other);
        void moveUnit(hexpoint_t from, hexpoint_t to);
        void attackUnit(hexpoint_t from, hexpoint_t to);
    };
}
#endif
