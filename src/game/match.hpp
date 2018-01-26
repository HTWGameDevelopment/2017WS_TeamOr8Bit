#ifndef TEAMOR8BIT_MATCH_HPP
#define TEAMOR8BIT_MATCH_HPP

#include<game/gameboard.hpp>
#include<game/unit.hpp>
#include<game/player.hpp>
#include<game/move.hpp>
#include<game/unitmanager.hpp>

namespace gamespace {
    class Match {
    private:
        GameBoard _board; //!< pointer to board
        std::vector<std::unique_ptr<Move>> _move;
        std::vector<std::function<void(Player&)>> _on_player_change;
        std::vector<std::function<void(Player&)>> _on_win;
        std::vector<std::function<void(Move&)>> _on_move;
        gamespace::Player _player1;
        gamespace::Player _player2;
        gamespace::Player *_currentPlayer;
        unsigned int _turn;
    public:
        Match(glm::ivec2 dim, Player player1, Player player2)
        : _board(dim.x, dim.y), _player1(player1), _player2(player2), _currentPlayer(&_player1) {
            setMarkers();
        }
        Match(const Match &other) = delete;
        Match(Match &&other): _board(std::move(other._board)), _move(std::move(other._move)), _player1(std::move(other._player1)), _player2(std::move(other._player2)), _currentPlayer(&_player1) {
            other._currentPlayer = nullptr;
        }
        GameBoard &board() {return _board;}
        Player &player1() {return _player1;}
        Player &player2() {return _player2;}
        Player &currentPlayer() {return *_currentPlayer;}
        void doMove(Move *m) {
            _move.emplace_back(m);
            m->doMove();
            ++_turn;
            for(size_t i = 0; i < _on_move.size(); ++i)
                _on_move[i](*m);
            checkWinningCondition();
        }
        void checkWinningCondition();
        unsigned int getTurnId() {
            return _turn;
        }
        void endTurn() {
            if(_currentPlayer == &_player1) _currentPlayer = &_player2;
            else _currentPlayer = &_player1;

            for(size_t i = 0; i < _on_player_change.size(); ++i)
                _on_player_change[i](currentPlayer());
        }
        template<typename F> void observe_player_change(F &&f) {
            _on_player_change.push_back(f);
            f(currentPlayer());
        }
        template<typename F> void on_win(F &&f) {
            _on_win.push_back(f);
        }
        template<typename F> void observe_moves(F &&f) {
            _on_move.push_back(f);
        }
        bool can_trigger_map_event() {
            return _board[7][13].unit() && _board[13][13].unit()
                && _board[7][13].unit()->player() == *_currentPlayer
                && _board[13][13].unit()->player() == *_currentPlayer;
        }
        void trigger_map_event() {
            for(size_t i = 0; i < _board.x(); ++i) {
                for(size_t j = 0; j < _board.y(); ++j) {
                    if(_board[i][j].marked_value(AOE_LAYER) == 1) {
                        _board[i][j].destroyUnit();
                    }
                }
            }
        }
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "Match" << std::endl;
            _board.__introspect(off + 2);
            for(size_t i = 0; i < _move.size(); ++i) {
                _move[i]->__introspect(off + 2);
            }
            _player1.__introspect(off + 2);
            _player2.__introspect(off + 2);
        }
        void setMarkers();
        void setRenderOffsets(UnitManager &manager);
    };
}


#endif //TEAMOR8BIT_MATCH_HPP
