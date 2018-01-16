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
        hexpoint_t from;
        hexpoint_t to;
        UnitMove::UnitMove(hexpoint_t f, hexpoint_t t) {
            from = f;
            to = t;
        };
        virtual std::string toString() = 0;
    };
}


#endif //OR8BIT_UNITMOVE_HPP
