//
// Created by simon on 23.11.2017.
//

#ifndef TEAMOR8BIT_MATCH_HPP
#define TEAMOR8BIT_MATCH_HPP

#include<game/player.hpp>
#include<game/move.hpp>

namespace gamespace {
    class Match {
        public:
        board* getBoard();
        move* getMove();
        player* getPlayer1();
        player* getPlayer2();

        private:
        hextile::HexTile<BoardTile> _board; // pointer to board
        std::vector<Move> _move;
        gamespace::Player _player1;
        gamespace::Player _player2;
    };
}


#endif //TEAMOR8BIT_MATCH_HPP
