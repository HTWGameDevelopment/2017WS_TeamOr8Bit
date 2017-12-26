#ifndef TEAMOR8BIT_MATCH_HPP
#define TEAMOR8BIT_MATCH_HPP

#include<game/gameboard.hpp>
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
        gamespace::Player *_currentPlayer;
    public:
        Match(glm::ivec2 dim, Player player1, Player player2)
        : _board(dim.x, dim.y), _player1(player1), _player2(player2), _currentPlayer(&_player1) {}
        Match(const Match &other) = delete;
        Match(Match &&other): _board(std::move(other._board)), _move(std::move(other._move)), _player1(std::move(other._player1)), _player2(std::move(other._player2)), _currentPlayer(&_player1) {
            other._currentPlayer = nullptr;
        }
        GameBoard &board() {return _board;}
        Player &player1() {return _player1;}
        Player &player2() {return _player2;}
        Player &currentPlayer() {return *_currentPlayer;}
        void endTurn() {
            if(_currentPlayer == &_player1) _currentPlayer = &_player2;
            else _currentPlayer = &_player1;
        }
    };
}


#endif //TEAMOR8BIT_MATCH_HPP
