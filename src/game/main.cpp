#include<engine/qe.hpp>

#include "board.hpp"
#include "scripting.hpp"

#include<logger.h>

#include<config.h>

#include<string.h>

#include <stdio.h>

/**
 * \brief Main game class
 */
class Game {
private:
    qe::Context *_ctxt;
    std::unique_ptr<qe::Mesh<qe::OBJV1>> _cube;
    std::unique_ptr<qe::Mesh<qe::OBJV2>> _tile;
    struct CameraData {
        std::unique_ptr<qe::Camera> camera;
    } _cam;
    struct Shaders {
        qe::Program objv1;
        qe::Program objv2;
    } _shaders;
    struct Textures {
        std::unique_ptr<qe::Texture<qe::PNGRGBA>> hextile_grass;
    } _textures;
    gamespace::GameBoard _board;
    gamespace::Scripting _scripting;
public:
    Game(qe::Context *ctxt): _ctxt(ctxt), _board(10, 5), _scripting(&_board) {
        assert(ctxt);
    }
    void initializeAssets() {
        // MODELS
        _cube.reset(new qe::Mesh<qe::OBJV1>(qe::Loader<qe::OBJV1>("assets/models/cube.objv1"_p)));
        _tile.reset(new qe::Mesh<qe::OBJV2>(qe::Loader<qe::OBJV2>("assets/models/hextile.objv2"_p)));
        // TEXTURES
        _textures.hextile_grass.reset(new qe::Texture<qe::PNGRGBA>(qe::Loader<qe::PNGRGBA>("assets/textures/hextile-grass.png"_p)));
        // SHADERS
        _shaders.objv1 = qe::mkProgram("assets/shaders/objv1.vsh"_p, "assets/shaders/objv1.fsh"_p);
        _shaders.objv2 = qe::mkProgram("assets/shaders/objv2.vsh"_p, "assets/shaders/objv2.fsh"_p);
        _shaders.objv2.use();
        // SETUP
        _shaders.objv2.setUniform<qe::UNIDIFFTEX>(qe::DIFFTEXBIND);
        _shaders.objv2.setUniform<qe::UNIL>(glm::vec3(0, -1, 0));

        _textures.hextile_grass->bindTo<GL_TEXTURE0>();

        _cam.camera.reset(new qe::Camera(
                              glm::vec3(4, 4, 4),
                              glm::vec2(-45, -45),
                              0.1,
                              30,
                              _ctxt->getAR(),
                              90
                          ));
        _scripting.loadScript("static/units/tank");
    }
    void run() {
        _shaders.objv2.use();
        unsigned int fps = 0;
        glm::mat4 m = glm::translate(glm::vec3(0, 0, 0));
        glClearColor(0.3, 0.3, 0.3, 1);

        while(!_ctxt->shouldClose()) {
            glm::mat4 mvp = _cam.camera->matrices().pv * m;
            _shaders.objv2.setUniform<qe::UNIMVP>(mvp);
            _shaders.objv2.setUniform<qe::UNIM>(m);
            _shaders.objv2.setUniform<qe::UNIV>(_cam.camera->matrices().v);
            _ctxt->start();
            render();
            _ctxt->swap();

            if(fps != _ctxt->fps()) {
                fps = _ctxt->fps();
                GINFO("FPS " << std::to_string(fps));
            }

            _ctxt->events();
        }
    }
    void render() {
        auto b = _board.begin();
        auto e = _board.end();

        for(; b != e; ++b) {
            glm::vec2 p = b->centerPos();
            glm::mat4 m = glm::translate(glm::vec3(p.x, 0, p.y));
            glm::mat4 mvp = _cam.camera->matrices().pv * m;
            _shaders.objv2.setUniform<qe::UNIMVP>(mvp);
            _shaders.objv2.setUniform<qe::UNIM>(m);
            _tile->render();
        }
    }
    qe::Camera *camera() {
        return _cam.camera.get();
    }
    qe::Context *context() {
        return _ctxt;
    }
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
    void keycallback(GLFWwindow *, int key, int, int action, int) {
        if(action == GLFW_REPEAT) return;

        if(key == GLFW_KEY_Q)
            game->context()->close();

        if(key == GLFW_KEY_P && action == GLFW_PRESS)
            qe::screenshot("screenshot.png", game->context()->width(), game->context()->height());

        if(key == GLFW_KEY_V && action == GLFW_PRESS)
            qe::VSYNC() = qe::VSYNC() == 0 ? 1 : 0;
        else if(key == GLFW_KEY_A) movementmask[MOVELEFT] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_D) movementmask[MOVERIGHT] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_W) movementmask[MOVEFORWARD] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_S) movementmask[MOVEBACKWARD] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_SPACE) movementmask[MOVEUP] = action == GLFW_PRESS;
        else if(key == GLFW_KEY_LEFT_SHIFT) movementmask[MOVEDOWN] = action == GLFW_PRESS;
    }

    void errorcallback(int, const char *desc) {
        GERR(desc);
    }

    void mousecallback(GLFWwindow *, double x, double y) {
        game->camera()->mouseMoved(game->context()->deltaT(), x, y);
        game->context()->resetMouse();
    }

    void idlecallback() {
        if(movementmask[MOVEUP]) game->camera()->moveUp(10 * game->context()->deltaT());

        if(movementmask[MOVEDOWN]) game->camera()->moveDown(10 * game->context()->deltaT());

        if(movementmask[MOVELEFT]) game->camera()->moveLeft(10 * game->context()->deltaT());

        if(movementmask[MOVERIGHT]) game->camera()->moveRight(10 * game->context()->deltaT());

        if(movementmask[MOVEFORWARD]) game->camera()->moveForward(10 * game->context()->deltaT());

        if(movementmask[MOVEBACKWARD]) game->camera()->moveBackward(10 * game->context()->deltaT());
    }

}

int main(int argc, char *argv[]) {
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--vsync") == 0) {
            qe::VSYNC() = 1;
        } else if(strcmp(argv[i], "--version") == 0) {
            std::cout << TONAME << " " << TOVERSION_STRING << " - (C) " << TOTNAME << ", 2017 All rights reserved." << std::endl;
            exit(0);
        }
    }

    qe::Context context(std::string(TOTNAME) + " - " + TONAME + " " + TOVERSION_STRING, 1920, 1080);
    context.meshcontext();
    {
        Game g(&context);
        game = &g;
        g.initializeAssets();
        g.run();
    }
    return 0;
}
