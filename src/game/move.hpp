#ifndef TEAMOR8BIT_MOVE_HPP
#define TEAMOR8BIT_MOVE_HPP

namespace gamespace {
    class Move {
    public:
        inline void doMove();
        inline void undoMove();
        virtual String toString();
    };
}


#endif //TEAMOR8BIT_MOVE_HPP
