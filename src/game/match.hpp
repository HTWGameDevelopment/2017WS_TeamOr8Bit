#ifndef TEAMOR8BIT_MATCH_HPP
#define TEAMOR8BIT_MATCH_HPP

#include<game/board.hpp>
#include<game/player.hpp>
#include<game/move.hpp>

namespace gamespace {
    class Match {
        private:
        hextile::HexTile<BoardTile> _board; //!< pointer to board
        std::vector<Move> _move;
        gamespace::Player _player1;
        gamespace::Player _player2;
        public:
        GameBoard &getBoard();
        Move &getMove();
        Player &getPlayer1();
        Player &getPlayer2();
    };
}


#endif //TEAMOR8BIT_MATCH_HPP
