#ifndef OR8BIT_TERRAIN_HPP
#define OR8BIT_TERRAIN_HPP

namespace gamespace {

    class Terrain {
    private:
        unsigned int _walk_falloff;
    public:
        Terrain(unsigned int wf): _walk_falloff(wf) {}
        unsigned int falloff() {
            return _walk_falloff;
        }
        static Terrain getNormalTile() {
            return Terrain(1);
        }
        static Terrain getWaterTile() {
            return Terrain(2);
        }
    };

}


#endif //OR8BIT_UNIT_HPP
