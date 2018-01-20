//
// Created by simon on 16.01.2018.
//

#ifndef OR8BIT_UNITMOVE_HPP
#define OR8BIT_UNITMOVE_HPP

#include<string>
#include<hextile/hextile.hpp>

namespace gamespace {
    class UnitMove {
    public:
        UnitMove(hexpoint_t f, hexpoint_t t) {
            from = f;
            to = t;
        };
        virtual ~UnitMove();
        UnitMove(const UnitMove &right);
        hexpoint_t getFrom() {
            return from;
        }
        hexpoint_t getTo() {
            return to;
        }
    private:
        hexpoint_t from;
        hexpoint_t to;
    };
}


#endif //OR8BIT_UNITMOVE_HPP
