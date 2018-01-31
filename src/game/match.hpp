#ifndef TEAMOR8BIT_MATCH_HPP
#define TEAMOR8BIT_MATCH_HPP

#include<game/gameboard.hpp>
#include<game/unit.hpp>
#include<game/player.hpp>
#include<game/move.hpp>
#include<game/unitmanager.hpp>

namespace gamespace {
    class Match {
    public:
        struct cb_player_t {
            unsigned int id;
            std::function<void(Player&)> f;
            cb_player_t(unsigned int i, std::function<void(Player&)> &&fu)
            : id(i)
            , f(std::move(fu)) {}
            cb_player_t(const cb_player_t &other) = delete;
            cb_player_t(cb_player_t &&other): id(other.id), f(std::move(other.f)) {}
            cb_player_t &operator=(const cb_player_t &other) = delete;
            cb_player_t &operator=(cb_player_t &&other) {
                id = other.id;
                f = std::move(other.f);
                return *this;
            }
        };
    private:
        GameBoard _board; //!< pointer to board
        std::vector<std::unique_ptr<Move>> _move;
        std::vector<cb_player_t> _on_player_change;
        std::vector<std::function<void(Player&)>> _on_win;
        std::vector<std::function<void(Move&)>> _on_move;
        gamespace::Player _player1;
        gamespace::Player _player2;
        gamespace::Player *_currentPlayer;
        unsigned int _turn;
        unsigned int _lid;
    public:
        Match(glm::ivec2 dim, Player player1, Player player2)
        : _board(dim.x, dim.y, this), _player1(player1), _player2(player2), _currentPlayer(&_player1), _turn(0), _lid(0) {
            setMarkers();
            _turn = 0;
        }
        Match(const Match &other) = delete;
        Match(Match &&other)
        : _board(std::move(other._board)), _move(std::move(other._move))
        , _on_player_change(std::move(other._on_player_change)), _on_win(std::move(other._on_win))
        , _on_move(std::move(other._on_move))
        , _player1(std::move(other._player1))
        , _player2(std::move(other._player2))
        , _currentPlayer(&_player1), _turn(0), _lid(other._lid) {
            other._currentPlayer = nullptr;
            _board.match() = this;
        }
        Match &operator=(const Match &other) = delete;
        Match &operator=(Match &&other) = delete;
        GameBoard &board() {return _board;}
        Player &player1() {return _player1;}
        Player &player2() {return _player2;}
        Player &currentPlayer() {return *_currentPlayer;}
        void doMove(Move *m) {
            _move.emplace_back(m);
            m->doMove();
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

            ++_turn;

            for(size_t i = 0; i < _on_player_change.size(); ++i)
                _on_player_change[i].f(currentPlayer());
        }
        template<typename F> unsigned int observe_player_change(F &&f) {
            _on_player_change.emplace_back(_lid, std::move(f));
            f(currentPlayer());
            return _lid++;
        }
        template<typename F> unsigned int on_player_change(F &&f) {
            _on_player_change.emplace_back(_lid, std::move(f));
            return _lid++;
        }
        void on_player_change_r(unsigned int id) {
            for(auto b = _on_player_change.begin(); b != _on_player_change.end(); ++b) {
                if(b->id == id) {
                    _on_player_change.erase(b);
                    return;
                }
            }
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
        void setDefaultUnits(UnitManager &manager);
        void setRenderOffsets(UnitManager &manager);
    };
}


#endif //TEAMOR8BIT_MATCH_HPP
