#include "gameinput.hpp"

#include<logger.h>

using namespace gamespace;

inline hextile::hexpoint_t getLookedAtTile(glm::vec3 _pc) {
    glm::vec2 pc(_pc.x, _pc.z);
    float yval1 = floor(pc.y / 1.5);
    float xval1 = floor((pc.x + (((int)yval1) % 2) * 0.5f * 2.0f * 0.866f) / (2.0f * 0.866f));
    glm::vec4 xval = glm::vec4(0, 1, 0, 1) + xval1;
    glm::vec4 yval = glm::vec4(0, 0, 1, 1) + yval1;

    glm::vec4 xpoints(2.0f * 0.866f * xval.x - ((int)yval.x % 2) * 0.5f * 2.0f * 0.866f,
        2.0f * 0.866f * xval.y - ((int)yval.y % 2) * 0.5f * 2.0f * 0.866f,
        2.0f * 0.866f * xval.z - ((int)yval.z % 2) * 0.5f * 2.0f * 0.866f,
        2.0f * 0.866f * xval.w - ((int)yval.w % 2) * 0.5f * 2.0f * 0.866f);
    glm::vec4 ypoints(yval * 1.5f);

    glm::vec4 distances(glm::distance(pc, glm::vec2(xpoints.x, ypoints.x)),
        glm::distance(pc, glm::vec2(xpoints.y, ypoints.y)),
        glm::distance(pc, glm::vec2(xpoints.z, ypoints.z)),
        glm::distance(pc, glm::vec2(xpoints.w, ypoints.w)));

    float d = std::min(distances.x, std::min(distances.y, std::min(distances.z, distances.w)));
    if(d == distances.x) return hextile::hexpoint_t {(int)xval.x, (int)yval.x};
    else if(d == distances.y) return hextile::hexpoint_t {(int)xval.y, (int)yval.y};
    else if(d == distances.z) return hextile::hexpoint_t {(int)xval.z, (int)yval.z};
    else if(d == distances.w) return hextile::hexpoint_t {(int)xval.w, (int)yval.w};
}

GameScreenInputState::GameScreenInputState(GameScreenImpl &impl): _mouse_mode(FREE), _resy(impl.context()->getResolution().y), _impl(&impl) {
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
    _last_x = x;
    _last_y = y;
    if(_mouse_mode == UIINTERACTION) {
        assert(_selected_menu);
        _selected_menu->origin() = _origin_save + ui::Point {x, _resy - y} - _mouse_save;
        ((CoordinateMenu*)_selected_menu->payload())->invalidate();
    } else {
        _impl->camera()->mouseMoved(_impl->context()->deltaT(), x, y, _mouse_mode == LOCKED);
        if(_mouse_mode == LOCKED) {
            _impl->context()->resetMouse();
        }
    }
}

void GameScreenInputState::button(int button, int action, int mods) {
    if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE && _mouse_mode == FREE) {
        _impl->context()->hideCursor();
        _impl->context()->resetMouse();
        _impl->inCameraMode(true);
        _mouse_mode = LOCKED;
        GDBG("new mouse mode: LOCKED");
        return;
    }
    if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT && _mouse_mode == FREE) {
        auto p = getLookedAtTile(_impl->camera()->getPlaneCoord());
        GDBG(GV2TOSTR(p));
        ui::Point s{_last_x, _resy - _last_y};
        if(_impl->ui()->click(s) == false) { // check for registered handlers first
            _selected_menu = _impl->ui()->hovers(s);
            if(_selected_menu) {
                _origin_save = _selected_menu->origin();
                _mouse_save = s;
                _mouse_mode = UIINTERACTION;
                GDBG("new mouse mode: UIINTERACTION");
            }
        }
        return;
    }
    if(action != GLFW_RELEASE) return;
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(_mouse_mode == UIINTERACTION) {
            _mouse_mode = FREE;
            _selected_menu = nullptr;
            GDBG("new mouse mode: FREE");
        } else if(_mouse_mode == FREE) _impl->enableMoveMask();
    } else if(button == GLFW_MOUSE_BUTTON_MIDDLE && _mouse_mode == LOCKED) {
        _impl->context()->displayCursor();
        _impl->context()->resetMouseToCenter();
        _impl->inCameraMode(false);
        _mouse_mode = FREE;
        GDBG("new mouse mode: FREE");
    } else if(button == GLFW_MOUSE_BUTTON_RIGHT && _mouse_mode == FREE) {
        _impl->createContextForLookAt();
    }
}

void GameScreenInputState::idle() {
    if(_movementmask[MOVEUP]) _impl->camera()->moveUp(10 * _impl->context()->deltaT());
    if(_movementmask[MOVEDOWN]) _impl->camera()->moveDown(10 * _impl->context()->deltaT());
    if(_movementmask[MOVELEFT]) _impl->camera()->moveLeft(10 * _impl->context()->deltaT());
    if(_movementmask[MOVERIGHT]) _impl->camera()->moveRight(10 * _impl->context()->deltaT());
    if(_movementmask[MOVEFORWARD]) _impl->camera()->moveForward(10 * _impl->context()->deltaT());
    if(_movementmask[MOVEBACKWARD]) _impl->camera()->moveBackward(10 * _impl->context()->deltaT());
}
