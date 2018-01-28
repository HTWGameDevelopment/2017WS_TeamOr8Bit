#ifndef GAMESPACE_BOARD_HPP
#define GAMESPACE_BOARD_HPP

#include<game/board.hpp>

#include<hextile/hextile.hpp>

#include<engine/buffer.hpp>

namespace gamespace {

    class Match;

    class GameBoard: public hextile::HexTile<BoardTile, 4> {
    private:
        Match *_match;
    public:
        GameBoard(int x, int y, Match *match);
        GameBoard(const GameBoard &other) = delete;
        GameBoard(GameBoard &&other);
        GameBoard &operator=(const GameBoard &other) = delete;
        GameBoard &operator=(GameBoard &&other) = delete;
        Match *&match() {
            return _match;
        }
        void moveUnit(hexpoint_t from, hexpoint_t to, bool from_container);
        void attackUnit(hexpoint_t from, hexpoint_t to);
        void containerMove(hexpoint_t from, hexpoint_t to, bool from_container);
        void synchronize();
    };
}
#endif
