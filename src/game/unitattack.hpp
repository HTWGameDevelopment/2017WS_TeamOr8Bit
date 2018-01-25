#ifndef OR8BIT_UNITATTACK_HPP
#define OR8BIT_UNITATTACK_HPP

#include<game/match.hpp>
#include<game/move.hpp>
#include<string>

namespace gamespace {

    const unsigned int MOVE_ATTACK = 0;

    class UnitAttack: public Move {
    private:
        hexpoint_t _from;
        hexpoint_t _to;
        Match *_match;
    public:
        UnitAttack(hexpoint_t from, hexpoint_t to, Match *b): _from(from), _to(to), _match(b) {}
        virtual void doMove() {
            _match->board().attackUnit(_from, _to);
            _match->board().get(_from).unit()->setLastTurnId(_match->getTurnId());
            _match->board().get(_from).unit()->emit_change();
            _match->board().get(_to).unit()->emit_change();
        }
        virtual unsigned int type() {
            return MOVE_ATTACK;
        }
        virtual void __introspect(size_t off) {
            // TODO
        }
    };

}


#endif //OR8BIT_UNITATTACK_HPP
