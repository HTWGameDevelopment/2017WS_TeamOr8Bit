#include "gameinput.hpp"

using namespace gamespace;

GameScreenInputState::GameScreenInputState(GameScreenImpl &impl): _impl(&impl) {
    _movementmask[0] = false;
    _movementmask[1] = false;
    _movementmask[2] = false;
    _movementmask[3] = false;
    _movementmask[4] = false;
    _movementmask[5] = false;
}

void GameScreenInputState::key(int key, int action) {
    if(action == GLFW_REPEAT) return;

    if(key == GLFW_KEY_Q && action == GLFW_RELEASE)
        _impl->deactivate();

    if(key == GLFW_KEY_P && action == GLFW_RELEASE)
        qe::screenshot("screenshot.png", _impl->context()->width(), _impl->context()->height());
    else if(key == GLFW_KEY_V && action == GLFW_RELEASE)
        qe::VSYNC() = qe::VSYNC() == 0 ? 1 : 0;
    else if(key == GLFW_KEY_PERIOD && action == GLFW_RELEASE)
        _impl->match().endTurn();
    else if(key == GLFW_KEY_A) _movementmask[MOVELEFT] = action == GLFW_PRESS;
    else if(key == GLFW_KEY_D) _movementmask[MOVERIGHT] = action == GLFW_PRESS;
    else if(key == GLFW_KEY_W) _movementmask[MOVEFORWARD] = action == GLFW_PRESS;
    else if(key == GLFW_KEY_S) _movementmask[MOVEBACKWARD] = action == GLFW_PRESS;
    else if(key == GLFW_KEY_SPACE) _movementmask[MOVEUP] = action == GLFW_PRESS;
    else if(key == GLFW_KEY_LEFT_SHIFT) _movementmask[MOVEDOWN] = action == GLFW_PRESS;
}

void GameScreenInputState::mouse(double x, double y) {
    _impl->camera()->mouseMoved(_impl->context()->deltaT(), x, y);
    _impl->context()->resetMouse();
}

void GameScreenInputState::button(int button, int action, int mods) {
    if(action != GLFW_RELEASE) return;
    if(button == GLFW_MOUSE_BUTTON_LEFT)
        _impl->enableMoveMask();
    else if(button == GLFW_MOUSE_BUTTON_RIGHT)
        _impl->enableAttackMask();
}

void GameScreenInputState::idle() {
    if(_movementmask[MOVEUP]) _impl->camera()->moveUp(10 * _impl->context()->deltaT());
    if(_movementmask[MOVEDOWN]) _impl->camera()->moveDown(10 * _impl->context()->deltaT());
    if(_movementmask[MOVELEFT]) _impl->camera()->moveLeft(10 * _impl->context()->deltaT());
    if(_movementmask[MOVERIGHT]) _impl->camera()->moveRight(10 * _impl->context()->deltaT());
    if(_movementmask[MOVEFORWARD]) _impl->camera()->moveForward(10 * _impl->context()->deltaT());
    if(_movementmask[MOVEBACKWARD]) _impl->camera()->moveBackward(10 * _impl->context()->deltaT());
}
