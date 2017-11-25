#ifndef OR8BIT_UNIT_HPP
#define OR8BIT_UNIT_HPP

#include<functional>
#include<game/board.hpp>
#include<game/terrain.hpp>

namespace gamespace {

    class Unit {
    private:
        hexpoint_t _pos; //!< Position
        unsigned int hp; //!< Health points
        unsigned int dp; //!< Defense points
        unsigned int ap; //!< Attack points
        unsigned int ar; //!< Attack range
        unsigned int vr; //!< Visibility range
        unsigned int dpt; //!< travel distance per turn
        std::function<unsigned int(Terrain &)> _a; //!< attack range relation
        std::function<unsigned int(Terrain &)> _d; //!< attack damage relation
        std::function<unsigned int(Terrain &)> _t; //!< travel distance relation
        std::function<unsigned int(Terrain &)> _v; //!< visibility relation
    public:
    };

}


#endif //OR8BIT_UNIT_HPP
