#ifndef TEAMOR8BIT_MOVE_HPP
#define TEAMOR8BIT_MOVE_HPP

#include<string>

namespace gamespace {
    class Move {
    public:
        virtual void doMove() = 0;
        virtual void undoMove() = 0;
        virtual std::string toString() = 0;
        virtual void __introspect(size_t off) = 0;
    };
}


#endif //TEAMOR8BIT_MOVE_HPP
