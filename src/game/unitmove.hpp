#ifndef OR8BIT_UNITMOVE_HPP
#define OR8BIT_UNITMOVE_HPP

#include<game/match.hpp>
#include<game/move.hpp>
#include<string>

namespace gamespace {

    const unsigned int MOVE_MOVE = 1;

    class UnitMove: public Move {
    private:
        hexpoint_t _from;
        hexpoint_t _to;
        Match *_match;
    public:
        UnitMove(hexpoint_t from, hexpoint_t to, Match *b): _from(from), _to(to), _match(b) {}
        virtual void doMove() {
            _match->board().moveUnit(_from, _to);
            _match->board().get(_to).unit()->setLastTurnId(_match->getTurnId());
            _match->board().get(_to).unit()->emit_change();
        }
        virtual unsigned int type() {
            return MOVE_MOVE;
        }
        virtual void __introspect(size_t off) {
            // TODO
        }
    };

}


#endif //OR8BIT_UNITMOVE_HPP
