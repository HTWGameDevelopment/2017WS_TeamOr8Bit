#include<engine/qe.hpp>

#include<logger.h>

#include<string.h>

class Game {
private:
    qe::Context *_ctxt;
    std::unique_ptr<qe::Mesh<qe::OBJV1>> _cube;
    struct CameraData {
        std::unique_ptr<qe::Camera> camera;
    } _cam;
    struct Shaders {
        qe::Program objv1;
    } _shaders;
public:
    Game(qe::Context *ctxt): _ctxt(ctxt) {
        assert(ctxt);
    }
    void initializeAssets() {
        _cube.reset(new qe::Mesh<qe::OBJV1>(qe::Loader<qe::OBJV1>("assets/models/cube.objv1")));
        _shaders.objv1 = qe::mkProgram("assets/shaders/objv1.vsh", "assets/shaders/objv1.fsh");
        _cam.camera.reset(new qe::Camera(
            glm::vec3(4, 4, 4),
            glm::vec2(-45, -45),
            _ctxt->getResolution(),
            0.1,
            30,
            _ctxt->getAR(),
            90
        ));
    }
    void run() {
        _shaders.objv1.use();
        unsigned int fps = 0;
        while(!_ctxt->shouldClose()) {
            glm::mat4 mvp = _cam.camera->matrices().pv * glm::translate(glm::vec3(0, 0, 0));
            _shaders.objv1.setUniform<qe::UNIMVP>(mvp);
            _ctxt->start();
            render();
            _ctxt->swap();
            _ctxt->events();
            if(fps != _ctxt->fps()) {
                fps = _ctxt->fps();
                GINFO("FPS " << std::to_string(fps));
            }
        }
    }
    void render() {
        _cube->render();
    }
    qe::Camera *camera() {return _cam.camera.get();}
    qe::Context *context() {return _ctxt;}
};

Game *game;

const unsigned int MOVEUP = 0;
const unsigned int MOVEDOWN = 1;
const unsigned int MOVELEFT = 2;
const unsigned int MOVERIGHT = 3;
const unsigned int MOVEFORWARD = 4;
const unsigned int MOVEBACKWARD = 5;

bool movementmask[6];

namespace qe {
    void keycallback(GLFWwindow*, int key, int, int action, int) {
        if(action == GLFW_REPEAT) return;
        if(key == GLFW_KEY_Q)
            game->context()->close();
        else if(key == GLFW_KEY_A) movementmask[MOVELEFT] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_D) movementmask[MOVERIGHT] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_W) movementmask[MOVEFORWARD] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_S) movementmask[MOVEBACKWARD] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_SPACE) movementmask[MOVEUP] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_LEFT_SHIFT) movementmask[MOVEDOWN] = action == GLFW_PRESS;
    }

    void errorcallback(int, const char* desc) {
        GERR(desc);
    }

    void mousecallback(GLFWwindow*, double x, double y) {
        game->camera()->mouseMoved(game->context()->deltaTN() * 30, x, y);
        game->context()->resetMouse();
    }

    void idlecallback() {
        if(movementmask[MOVEUP]) game->camera()->moveUp(10 * game->context()->deltaTN());
        if(movementmask[MOVEDOWN]) game->camera()->moveDown(10 * game->context()->deltaTN());
        if(movementmask[MOVELEFT]) game->camera()->moveLeft(10 * game->context()->deltaTN());
        if(movementmask[MOVERIGHT]) game->camera()->moveRight(10 * game->context()->deltaTN());
        if(movementmask[MOVEFORWARD]) game->camera()->moveForward(10 * game->context()->deltaTN());
        if(movementmask[MOVEBACKWARD]) game->camera()->moveBackward(10 * game->context()->deltaTN());
    }

}

int main() {
    qe::Context context("Team Or8Bit - Unnamed Game", 800, 600);
    context.meshcontext();
    {
        Game g(&context);
        game = &g;
        g.initializeAssets();
        g.run();
    }
    return 0;
}
