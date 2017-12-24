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
    std::unique_ptr<font::Font> _font;
    std::unique_ptr<qe::Mesh<qe::OBJV1>> _cube;
    std::unique_ptr<qe::Mesh<qe::OBJV2>> _tile;
    std::unique_ptr<qe::Mesh<qe::OBJV3>> _tank;
    struct CameraData {
        std::unique_ptr<qe::Camera> camera;
    } _cam;
    struct Textures {
        std::unique_ptr<qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>> hextile_grass;
    } _textures;
    struct Text {
        qe::Text<qe::GlyphmapLatin> gamename;
        qe::Text<qe::GlyphmapLatin> help1;
        qe::Text<qe::GlyphmapLatin> help2;
        std::string help2_tmp;
    } _strings;
    struct SelectionState {
        enum Type { SEL_TO_MOVE, SEL_TO_ATTACK, SEL_NONE };
        Type type;
        gamespace::BoardTile *selected;
        gamespace::BoardTile *hovering;
    } _selection;
    gamespace::Match _match;
    screen::ScreenManager<gamespace::Screen> _screens;
    gamespace::MainScreen *_mainmenu;
public:
    Game(qe::Context *ctxt)
    : _ctxt(ctxt), _font(font::Font::get("assets/fonts/DejaVuSans.ttf"_p)),
        _match(glm::ivec2(15, 8),
                gamespace::Player(glm::vec3(0.448, 0.884, 1), "Blue"),
                gamespace::Player(glm::vec3(1, 0.448, 0.448), "Red")) {
        assert(ctxt);
        _selection.selected = nullptr;
        _selection.hovering = nullptr;
        _selection.type = SelectionState::Type::SEL_NONE;
#ifdef HAS_FREETYPE
        std::cout << "Using font " << _font->bpath() << std::endl;
        qe::Cache::glyphlatin = new qe::GlyphmapLatin(_font->bpath(), _font->face(), 32, _ctxt->getResolution());
#else
        std::cout << "Using baked font " << _font->bpath() << std::endl;
        qe::Cache::glyphlatin = new qe::GlyphmapLatin(_font->bpath(), _ctxt->getResolution());
#endif
        _strings.gamename = qe::Text<qe::GlyphmapLatin>(qe::Cache::glyphlatin, glm::ivec2(500, 100), qe::PositionMode::TOP);
        _strings.help1 = qe::Text<qe::GlyphmapLatin>(qe::Cache::glyphlatin, glm::ivec2(500, 150), qe::PositionMode::TOP);
        _strings.help2 = qe::Text<qe::GlyphmapLatin>(qe::Cache::glyphlatin, glm::ivec2(500, 200), qe::PositionMode::TOP);
        _strings.gamename.text() = "Or8Bit - (c) 2017 Team Or8Bit";
        _strings.help1.text() = "LMB to move, RMB to attack";
        _strings.help2_tmp = "Current player (. for ending a turn): ";
        _strings.help2.text() = _strings.help2_tmp + _match.currentPlayer().name();
    }
    void initializeAssets() {
        // MODELS
        _cube.reset(new qe::Mesh<qe::OBJV1>(qe::Loader<qe::OBJV1>("assets/models/cube.objv1"_p)));
        _tile.reset(new qe::Mesh<qe::OBJV2>(qe::Loader<qe::OBJV2>("assets/models/hextile.objv2"_p)));
        _tank.reset(new qe::Mesh<qe::OBJV3>(qe::Loader<qe::OBJV3>("assets/models/tank.objv3"_p)));
        qe::Cache::meshm = new qe::Mesh<qe::TEXTG>();
        // TEXTURES
        _textures.hextile_grass.reset(new qe::Texture<qe::PNGRGBA, qe::DIFFTEXBIND_GL>(qe::Loader<qe::PNGRGBA>("assets/textures/hextile-grass.png"_p)));
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

        _cam.camera.reset(new qe::Camera(
                              glm::vec3(4, 4, 4),
                              glm::vec2(-45, -45),
                              0.1,
                              30,
                              _ctxt->getAR(),
                              90
                          ));
    }
    void initializeScreens() {
        auto *a = _screens.addScreen(new gamespace::AboutScreen());
        auto *g = _screens.addScreen(new gamespace::GameScreen());
        _mainmenu = _screens.addScreen(new gamespace::MainScreen(_ctxt, *g));

        _mainmenu->linkAbout(a);
        _mainmenu->linkGame(g);
    }
    gamespace::Match &match() {
        return _match;
    }
    void initializeMap() {
        auto b = _match.board().begin();
        auto e = _match.board().end();

        for(; b != e; ++b) {
            b->setUnit(nullptr);
        }

        auto *u1 = new gamespace::Unit(_tank.get(), &_match.player1(),
            100,
            50,
            50,
            3,
            3,
            2,
            gamespace::defaultFalloff,
            gamespace::defaultFalloff,
            gamespace::defaultFalloff,
            gamespace::defaultFalloff);
        auto *u2 = new gamespace::Unit(_tank.get(), &_match.player2(),
            100,
            50,
            50,
            3,
            3,
            2,
            gamespace::defaultFalloff,
            gamespace::defaultFalloff,
            gamespace::defaultFalloff,
            gamespace::defaultFalloff);
        // TESTING PURPOSES. MOVE THIS TO MATCH SOMEHOW
        _match.board()[0][0].setUnit(new gamespace::Unit(*u1));
        _match.board()[1][0].setUnit(new gamespace::Unit(*u1));
        _match.board()[0][1].setUnit(new gamespace::Unit(*u1));
        _match.board()[1][1].setUnit(new gamespace::Unit(*u1));

        _match.board()[0][5].setUnit(new gamespace::Unit(*u2));
        _match.board()[1][5].setUnit(new gamespace::Unit(*u2));
        _match.board()[0][6].setUnit(new gamespace::Unit(*u2));
        _match.board()[1][6].setUnit(new gamespace::Unit(*u2));

        delete u1;
        delete u2;

        // _match.board()[0][0].unit()->markVisibility(_match.board()[0][0]);
    }
    void bakeAssets() {
        qe::Cache::glyphlatin->bake();
    }
    void enableMoveMask() {
        if(_selection.hovering == nullptr) return; // no tile to select
        if(_selection.type == SelectionState::Type::SEL_TO_MOVE) {
            if(_selection.hovering->marked() && _selection.hovering != _selection.selected) {
                assert(_selection.selected);
                if(_selection.hovering->unit() != nullptr) return; // cannot move to an occupied tile
                _match.board().moveUnit(_selection.selected->coord(), _selection.hovering->coord());
            }
            _selection.type = SelectionState::Type::SEL_NONE;
            _selection.selected = nullptr;
            _selection.hovering = nullptr;
            _match.board().clearMarker();
        } else {
            if(_selection.hovering->unit() == nullptr) return; // no unit to select
            if(_selection.hovering->unit()->player() != _match.currentPlayer()) return; // can only select own units
            _selection.hovering->unit()->markMovement(*_selection.hovering);
            _selection.selected = _selection.hovering;
            _selection.type = SelectionState::Type::SEL_TO_MOVE;
        }
    }
    void enableAttackMask() {
        if(_selection.hovering == nullptr) return; // no tile to select
        if(_selection.type == SelectionState::Type::SEL_TO_ATTACK) {
            if(_selection.hovering->marked() && _selection.hovering != _selection.selected) {
                assert(_selection.selected);
                if(_selection.hovering->unit() == nullptr) return; // cannot attack empty tile
                if(_selection.hovering->unit()->player() == _match.currentPlayer()) return; // cannot attack own unit
                _match.board().attackUnit(_selection.selected->coord(), _selection.hovering->coord());
            }
            _selection.type = SelectionState::Type::SEL_NONE;
            _selection.selected = nullptr;
            _selection.hovering = nullptr;
            _match.board().clearMarker();
        } else {
            if(_selection.hovering->unit() == nullptr) return; // no unit to select
            if(_selection.hovering->unit()->player() != _match.currentPlayer()) return; // can only select own units
            _selection.hovering->unit()->markAttack(*_selection.hovering);
            _selection.selected = _selection.hovering;
            _selection.type = SelectionState::Type::SEL_TO_ATTACK;
        }
    }
    bool isLookedAtTile(glm::vec2 ori, glm::vec3 planecoord) {
        glm::vec2 pc(planecoord.x, planecoord.z);
        auto dx = 0.5 * gamespace::BoardTile::dim_x;
        auto dy = 0.5 * gamespace::BoardTile::dim_y;
        pc += glm::vec2(dx, dy);
        glm::vec2 edge1 = glm::normalize(glm::vec2(0, 1));
        glm::vec2 edge2 = glm::normalize(glm::vec2(dx, 0.5));
        glm::vec2 edge3 = glm::normalize(glm::vec2(dx, -0.5));
        glm::vec2 edge4 = glm::normalize(glm::vec2(0,-1));
        glm::vec2 edge5 = glm::normalize(glm::vec2(-dx, 0.5));
        glm::vec2 edge6 = glm::normalize(glm::vec2(-dx,-0.5));
        if(abs(glm::dot(edge1, ori - pc)) > 0.7) return false;
        if(abs(glm::dot(edge2, ori - pc)) > 0.7) return false;
        if(abs(glm::dot(edge3, ori - pc)) > 0.7) return false;

        if(abs(glm::dot(edge4, ori - pc)) > 0.7) return false;
        if(abs(glm::dot(edge5, ori - pc)) > 0.7) return false;
        if(abs(glm::dot(edge6, ori - pc)) > 0.7) return false;

        return true;
    }
    void run() {
        //_screens.changeActiveScreen(*_mainmenu); // display main menu
        unsigned int fps = 0;
        glm::mat4 m = glm::translate(glm::vec3(0, 0, 0));
        glClearColor(0.3, 0.3, 0.3, 1);

        _textures.hextile_grass->bindTo();

        while(!_ctxt->shouldClose()) {
            // calculate lookat tile
            glm::mat4 mvp = _cam.camera->matrices().pv * m;
            qe::Cache::objv2->use();
            qe::Cache::objv2->setUniform<qe::UNIMVP>(mvp);
            qe::Cache::objv2->setUniform<qe::UNIM>(m);
            qe::Cache::objv2->setUniform<qe::UNIV>(_cam.camera->matrices().v);
            qe::Cache::objv3->use();
            qe::Cache::objv3->setUniform<qe::UNIV>(_cam.camera->matrices().v);
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
        auto b = _match.board().begin();
        auto e = _match.board().end();
        std::array<gamespace::BoardTile*, 3> selected;
        uint8_t len = 0;

        // render tiles and units
        for(; b != e; ++b) {
            if(isLookedAtTile(b->centerPos(), _cam.camera->getPlaneCoord())) {
                selected[len++] = &*b;
            } else {
                renderTile(&*b, glm::vec3(0, 0, 0));
            }
        }

        _selection.hovering = nullptr;
        if(len == 1) { // render one selected
            _selection.hovering = selected[0];
            renderTile(selected[0], glm::vec3(0.2, 0.2, 0.2));
        } else if(len != 0){
            for(uint8_t i = 0; i < len; ++i) renderTile(selected[i], glm::vec3(0, 0, 0));
        }

        // render text
        _ctxt->textcontext();
        _strings.gamename.render();
        _strings.help1.render();
        _strings.help2.text() = _strings.help2_tmp + _match.currentPlayer().name();
        _strings.help2.render();
        _ctxt->meshcontext();
    }
    void renderTile(gamespace::BoardTile *b, glm::vec3 ho) {
        glm::vec2 p = b->centerPos();
        if(b->marked()) {
            if(_selection.type == SelectionState::Type::SEL_NONE) _match.board().clearMarker();
            else ho += glm::vec3(0.2, 0.2, 0.2);
        }
        glm::mat4 m = glm::translate(glm::vec3(p.x, -0.25, p.y));
        glm::mat4 mvp = _cam.camera->matrices().pv * m;
        qe::Cache::objv2->use();
        qe::Cache::objv2->setUniform<qe::UNIMVP>(mvp);
        qe::Cache::objv2->setUniform<qe::UNIM>(m);
        qe::Cache::objv2->setUniform<qe::UNICOLOR>(ho);
        _tile->render();
        // render unit
        if(b->unit() != nullptr) {
            m = glm::translate(glm::vec3(p.x, 0, p.y));
            mvp = _cam.camera->matrices().pv * m;
            b->unit()->render(*b, mvp, m);
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

        if(key == GLFW_KEY_P && action == GLFW_RELEASE)
            qe::screenshot("screenshot.png", game->context()->width(), game->context()->height());
        else if(key == GLFW_KEY_V && action == GLFW_RELEASE)
            qe::VSYNC() = qe::VSYNC() == 0 ? 1 : 0;
        else if(key == GLFW_KEY_PERIOD && action == GLFW_RELEASE)
            game->match().endTurn();
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

    void mousebuttoncallback(GLFWwindow *, int button, int action, int mods) {
        if(action != GLFW_RELEASE) return;
        if(button == GLFW_MOUSE_BUTTON_LEFT)
            game->enableMoveMask();
        else if(button == GLFW_MOUSE_BUTTON_RIGHT)
            game->enableAttackMask();
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
        } else if(strcmp(argv[i], "--bake") == 0) {
            qe::BAKEFONTS() = true;
        }
    }

    qe::Context context(std::string(TOTNAME) + " - " + TONAME + " " + TOVERSION_STRING, 1920, 1080);
    context.meshcontext();
    {
        Game g(&context);
        game = &g;
        g.initializeAssets();
        g.initializeScreens();
        g.initializeMap();

        if(qe::BAKEFONTS()) g.bakeAssets();

        g.run();
    }
    return 0;
}
