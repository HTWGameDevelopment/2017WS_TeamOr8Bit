#ifndef GAMESPACE_MAINMENU
#define GAMESPACE_MAINMENU

#include<engine/cache.hpp>
#include<engine/context.hpp>

#include<game/screens/screen.hpp>
#include<game/screens/about.hpp>
#include<game/screens/game.hpp>

#include<ui/abstractbox.hpp>
#include<ui/abstracttext.hpp>
#include<ui/uifactory.hpp>
#include<screenmanager/screen.hpp>

namespace gamespace {
    class MainScreen: public Screen {
    private:
        AboutScreen *_about;
        GameScreen *_game;
        qe::Context *_ctxt;
        GameScreen *_gamescreen;
        screen::ScreenManager<Screen> *_manager;
        bool _active;
        glm::dvec2 _save;
        std::unique_ptr<ui::DefinedUI> _ui;
    public:
        MainScreen(screen::ScreenManager<Screen> &manager, qe::Context *ctxt, GameScreen &gamescreen): Screen(), _ctxt(ctxt), _gamescreen(&gamescreen), _manager(&manager) {
            assert(ctxt);
            _save = _ctxt->getCenterCoordinate();
            load_ui();
        }
        void activate_screen() {
            _active = true;
            qe::Cache::sprite2d->use();
            _ctxt->displayCursor(_save);
            while(_active) {
                _ctxt->start();
                _ui->render();
                //render_background();
                //render_buttons();
                _ctxt->swap();
                process_events();
            }
        }
        void deactivate_screen() {
            _save = _ctxt->getMousePos();
            _active = false;
        }
        void linkAbout(AboutScreen *about) {
            _about = about;
        }
        void linkGame(GameScreen *game) {
            _game = game;
        }
        void render_buttons() {
            render_button(glm::vec2(-0.25, -0.25), glm::vec2(0.5, 0.25));
            render_button(glm::vec2(-0.25, -0.75), glm::vec2(0.5, 0.25));
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
        void render_button(ui::defp_t a, ui::defp_t b) {
            render_button(glm::vec2(a.x, a.y) * glm::vec2(2, 2) / _ctxt->getResolution() - glm::vec2(1, 1),
                glm::vec2(b.x, b.y) * glm::vec2(2, 2) / _ctxt->getResolution());
        }
        void render_background() {
            // TODO
        }
        void process_events() {
            _ctxt->waitEvents();
        }
        virtual void mouse_button_callback(int button, int action, int mods) {
            glm::ivec2 cpos = _ctxt->getMousePos();
            _ui->click(ui::defp_t {(int)cpos.x, (int)(_ctxt->getResolution().y - cpos.y)});
        }
        void key_callback(int key, int action) {}
        void mouse_callback(double x, double y) {}
        void idle_callback() {}
        void load_ui() {
            ui::AbstractUI ui;
            std::unique_ptr<ui::AbstractBox> box(new ui::AbstractBox());
            std::unique_ptr<ui::AbstractText> text1(new ui::AbstractText());
            std::unique_ptr<ui::AbstractText> text2(new ui::AbstractText());

            text1->dimension() = ui::absp_t {0.15, 0.15};
            text2->dimension() = ui::absp_t {0.15, 0.15};
            text1->margin() = ui::absp_t {0.02, 0.02};
            text2->margin() = ui::absp_t {0.02, 0.02};

            box->set_orientation(ui::AbstractBox::Orientation::VERTICAL);
            box->set_growth(ui::AbstractBox::Growth::FILL);
            box->set_align_inner(ui::AbstractBox::Align::CENTER, ui::AbstractBox::Align::CENTER);

            box->append(text2.release());
            box->append(text1.release());
            ui.set_container(box.release());

            _ui.reset(new ui::DefinedUI(ui::UIFactory(ui, _ctxt->width(), _ctxt->height()).release()));
            auto *quit = _ui->get("1.1");
            auto *start = _ui->get("1.2");
            start->on_click([this](void) mutable {
                _gamescreen->newGame(glm::ivec2(15, 8),
                    gamespace::Player(glm::vec3(0.448, 0.884, 1), "Blue"),
                    gamespace::Player(glm::vec3(1, 0.448, 0.448), "Red"));
                _manager->changeActiveScreen(*_gamescreen);
            });
            quit->on_click([this](void) mutable {
                _manager->quit();
            });
            start->render_with([this, start](void) mutable {this->render_button(start->origin() + start->margin(), start->dimension() - start->margin());});
            quit->render_with([this, quit](void) mutable {this->render_button(quit->origin() + quit->margin(), quit->dimension() - quit->margin());});
#ifndef NDEBUG
            _ui->debug();
#endif
        }
    };
}

#endif
