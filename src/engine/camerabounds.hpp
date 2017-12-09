#ifndef CAMERABOUNDS_HPP
#define CAMERABOUNDS_HPP

#include<glm/glm.hpp>
namespace qe {
    class CameraBounds {
    public:
        const float maxHeight;
        const float minHeight = 0;
        const float maxLeft;
        const float maxRight;
        const float maxTop;
        const float maxDown;
        const glm::vec2 maxAngle;
        const glm::vec2 minAngle;
    };

}
#endif
