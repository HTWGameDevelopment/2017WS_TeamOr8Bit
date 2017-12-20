#ifndef TEAMOR8BIT_MATCH_HPP
#define TEAMOR8BIT_MATCH_HPP

#include<game/board.hpp>
#include<game/unit.hpp>
#include<game/player.hpp>
#include<game/move.hpp>

namespace gamespace {
    class Match {
    private:
        GameBoard _board; //!< pointer to board
        std::vector<Move> _move;
        gamespace::Player _player1;
        gamespace::Player _player2;
    public:
        Match(glm::ivec2 dim, Player player1, Player player2)
        : _board(dim.x, dim.y), _player1(player1), _player2(player2) {}
        GameBoard &board() {return _board;}
        Player &player1() {return _player1;}
        Player &player2() {return _player2;}
    };
}


#endif //TEAMOR8BIT_MATCH_HPP
