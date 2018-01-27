#ifndef OR8BIT_UNITCMOVE_HPP
#define OR8BIT_UNITCMOVE_HPP

#include<game/match.hpp>
#include<game/move.hpp>
#include<string>

namespace gamespace {

    const unsigned int CONTAINER_MOVE = 2;

    class UnitCMove: public Move {
    private:
        hexpoint_t _from;
        hexpoint_t _to;
        Match *_match;
        bool _fc;
    public:
        UnitCMove(hexpoint_t from, hexpoint_t to, Match *b, bool from_container): _from(from), _to(to), _match(b), _fc(from_container) {}
        virtual void doMove() {
            _match->board().containerMove(_from, _to, _fc);
            if(_fc) {
                _match->board().get(_from).unit()->setLastTurnId(_match->getTurnId());
                _match->board().get(_from).unit()->emit_change();
            }
            _match->board().get(_to).unit()->setLastTurnId(_match->getTurnId());
            _match->board().get(_to).unit()->emit_change();
            _match->board().get(_to).unit()->container()->emit_change();
        }
        virtual unsigned int type() {
            return CONTAINER_MOVE;
        }
        virtual void __introspect(size_t off) {
            // TODO
        }
    };

}


#endif //OR8BIT_UNITMOVE_HPP
