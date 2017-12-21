#ifndef CAMERABOUNDS_HPP
#define CAMERABOUNDS_HPP

#include<glm/glm.hpp>
namespace qe {
    class CameraBounds {
    public:
        const float maxHeight;
        const float minHeight;
        const float maxLeft;
        const float maxRight;
        const float maxTop;
        const float maxDown;
        const glm::vec2 maxAngle;
        const glm::vec2 minAngle;
        CameraBounds():
            maxHeight(0),
            minHeight(0),
            maxLeft(0),
            maxRight(0),
            maxTop(0),
            maxDown(0),
            maxAngle(glm::vec2(0,0)),
            minAngle(glm::vec2(0,0)) {}
    };

}
#endif
