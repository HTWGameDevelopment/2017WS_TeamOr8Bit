#ifndef GAMESPACE_MAINMENU
#define GAMESPACE_MAINMENU

#include<engine/cache.hpp>
#include<engine/context.hpp>

#include<game/screens/screen.hpp>
#include<game/screens/about.hpp>
#include<game/screens/game.hpp>

namespace gamespace {
    class MainScreen: public Screen {
    private:
        AboutScreen *_about;
        GameScreen *_game;
        qe::Context *_ctxt;
        GameScreen *_gamescreen;
        bool _active;
    public:
        MainScreen(qe::Context *ctxt, GameScreen &gamescreen): Screen(), _ctxt(ctxt), _gamescreen(&gamescreen) {
            assert(ctxt);
        }
        void activate_screen() {
            _active = true;
            qe::Cache::sprite2d->use();
            while(_active) {
                render_background();
                render_buttons();
                process_events();
            }
        }
        void deactivate_screen() {
            _active = false;
        }
        void linkAbout(AboutScreen *about) {
            _about = about;
        }
        void linkGame(GameScreen *game) {
            _game = game;
        }
        void render_buttons() {
            render_button(glm::vec2(-0.25, 0), glm::vec2(0.5, 0.25));
        }
        // TODO text rendering
        void render_button(glm::vec2 origin, glm::vec2 size) {
            // set shader
            qe::Cache::sprite2d->setUniform<qe::UNIORIGIN>(origin);
            qe::Cache::sprite2d->setUniform<qe::UNISIZE>(size);
            // set texture
            qe::Cache::buttont->bindTo();
            // set vao
            qe::Cache::meshm->render();
        }
        void render_background() {
            // TODO
        }
        void process_events() {
            _ctxt->waitEvents();
        }
        void mouse_button_callback(int button, int action, int mods) {
            glm::dvec2 cpos = _ctxt->absToRel(_ctxt->getMousePos());
            if(cpos.x >= -0.25 && cpos.x <= 0.25 && cpos.y >= 0 && cpos.y <= 0.25) {
                _gamescreen->newGame();
            }
        }
    };
}

#endif
