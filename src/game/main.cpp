#include<engine/qe.hpp>
#include<font/font.hpp>

#include "match.hpp"

#include<logger.h>

#include<config.h>

#include<game/screens/screens.hpp>
#include<screenmanager/screen.hpp>

#include<string.h>

#include <stdio.h>

/**
 * \brief Main game class
 */
class Game {
private:
    qe::Context *_ctxt;
    std::shared_ptr<font::Font> _font;
    screen::ScreenManager<gamespace::Screen> _screens;
    gamespace::MainScreen *_mainmenu;
public:
    Game(qe::Context *ctxt)
    : _ctxt(ctxt), _font(font::Font::get("assets/fonts/DejaVuSans.ttf"_p)) {
        assert(ctxt);
        initializeCacheAssets();
    }
    void initializeScreens() {
        auto *a = _screens.addScreen(new gamespace::AboutScreen());
        auto *g = _screens.addScreen(new gamespace::GameScreen(_screens, *_ctxt, _font));
        _mainmenu = _screens.addScreen(new gamespace::MainScreen(_screens, _ctxt, *g));

        _mainmenu->linkAbout(a);
        _mainmenu->linkGame(g);
    }
    void bakeAssets() {
        qe::Cache::glyphlatin->bake();
    }
    void run() {
        _screens.changeActiveScreen(*_mainmenu); // display main menu
        while(!_ctxt->shouldClose() && _screens.stopped() == false) {
            _screens.nextScreen();
        }
    }
    void key(int key, int action) {
        _screens.input_key(key, action);
    }
    void mouse(double x, double y) {
        _screens.input_mouse(x, y);
    }
    void button(int button, int action, int mods) {
        _screens.input_button(button, action, mods);
    }
    void idle() {
        _screens.input_idle();
    }
    qe::Context *context() {
        return _ctxt;
    }
    void initializeCacheAssets() {
        qe::Cache::meshm = new qe::Mesh<qe::TEXTG>();
        // SHADERS
        qe::Cache::objv1 = qe::mkProgram("assets/shaders/objv1.vsh"_p, "assets/shaders/objv1.fsh"_p);
        qe::Cache::objv2 = qe::mkProgram("assets/shaders/objv2.vsh"_p, "assets/shaders/objv2.fsh"_p);
        qe::Cache::objv3 = qe::mkProgram("assets/shaders/objv3.vsh"_p, "assets/shaders/objv3.fsh"_p);
        qe::Cache::texts = qe::mkProgram("assets/shaders/texts.vsh"_p, "assets/shaders/texts.fsh"_p);
        qe::Cache::sprite2d = qe::mkProgram("assets/shaders/s2d.vsh"_p, "assets/shaders/s2d.fsh"_p);
        qe::Cache::buttont = new qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>(qe::Loader<qe::PNGRGBA>("assets/textures/button.png"_p));
        // SETUP
        qe::Cache::objv2->use();
        qe::Cache::objv2->setUniform<qe::UNIDIFFTEX>(qe::DIFFTEXBIND);
        qe::Cache::objv2->setUniform<qe::UNIL>(glm::vec3(0.1, -1, 0.1));
        qe::Cache::objv3->use();
        qe::Cache::objv3->setUniform<qe::UNIL>(glm::vec3(0.1, -1, 0.1));
        qe::Cache::texts->use();
        qe::Cache::texts->setUniform<qe::UNICOLOR>(qe::FONTMAPBIND);
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
        game->key(key, action);
    }

    void errorcallback(int, const char *desc) {
        GERR(desc);
    }

    void mousecallback(GLFWwindow *, double x, double y) {
        game->mouse(x, y);
    }

    void mousebuttoncallback(GLFWwindow *, int button, int action, int mods) {
        game->button(button, action, mods);
    }

    void idlecallback() {
        game->idle();
    }

}

int main(int argc, char *argv[]) {
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--vsync") == 0) {
            qe::VSYNC() = 1;
        } else if(strcmp(argv[i], "--version") == 0) {
            std::cout << TONAME << " " << TOVERSION_STRING << " - (C) " << TOTNAME << ", 2017 All rights reserved." << std::endl;
            exit(0);
        } else if(strcmp(argv[i], "--bake") == 0) {
            qe::BAKEFONTS() = true;
        }
    }

    qe::Context context(std::string(TOTNAME) + " - " + TONAME + " " + TOVERSION_STRING, 1920, 1080);
    context.meshcontext();
    {
        Game g(&context);
        game = &g;
        g.initializeScreens();

        if(qe::BAKEFONTS()) g.bakeAssets();

        g.run();
    }
    return 0;
}
