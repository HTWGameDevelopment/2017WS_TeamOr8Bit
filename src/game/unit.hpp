//
// Created by simon on 25.11.2017.
//

#ifndef OR8BIT_UNIT_HPP
#define OR8BIT_UNIT_HPP

namespace gamespace {

    class Unit {
    private:
        //faction f;
        int posX;
        int posY;
        int h;
        int dp;
        int ap;
        int ar;
        int vr;
        int dpt;

        std::getMaxTravelDist();
        std::getMaxAttackRange();
        std::getAttackDamage();
        std::getVisibility();
    public:
        void unit(int x, int y) {}

    };

}


#endif //OR8BIT_UNIT_HPP
