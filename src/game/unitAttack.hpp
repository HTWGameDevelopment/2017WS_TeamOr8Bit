//
// Created by simon on 16.01.2018.
//

#ifndef OR8BIT_UNITATTACK_HPP
#define OR8BIT_UNITATTACK_HPP

#include<string>

namespace gamespace {
    class UnitAttack {
    public:
        virtual void doAttack() = 0;
        virtual void undoAttack() = 0;
        virtual std::string toString() = 0;
    };
}


#endif //OR8BIT_UNITATTACK_HPP
