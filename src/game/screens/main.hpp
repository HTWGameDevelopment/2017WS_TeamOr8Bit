#ifndef GAMESPACE_MAINMENU
#define GAMESPACE_MAINMENU

#include<engine/cache.hpp>
#include<engine/context.hpp>
#include<engine/text.hpp>

#include<game/screens/screen.hpp>
#include<game/screens/about.hpp>
#include<game/screens/game.hpp>

#include<ui/ui.hpp>
#include<ui/box.hpp>
#include<ui/text.hpp>
#include<screenmanager/screen.hpp>
#include<game/uirender.hpp>


namespace gamespace {

    typedef decltype(qe::Cache::glyphlatin) glyphmap;
    typedef qe::Text<std::remove_pointer<glyphmap>::type> text_t;

    class MainScreen: public Screen {
    private:
        AboutScreen *_about;
        qe::Context *_ctxt;
        GameScreen *_gamescreen;
        glyphmap _glyphmap;
        screen::ScreenManager<Screen> *_manager;
        bool _active;
        glm::dvec2 _save;
        std::unique_ptr<ui::UI> _ui;
    public:
        MainScreen(screen::ScreenManager<Screen> &manager, qe::Context *ctxt, glyphmap glyphmap): Screen(), _ctxt(ctxt), _gamescreen(nullptr), _glyphmap(glyphmap), _manager(&manager) {
            assert(ctxt);
            _save = _ctxt->getCenterCoordinate();
            load_ui();
        }
        void activate_screen() {
            _active = true;
            _ctxt->displayCursor(_save);
            _ctxt->textcontext();
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
            _gamescreen = game;
        }
        void render_button(glm::vec2 origin, glm::vec2 size) {
            qe::Cache::sprite2d->use();
            // set shader
            qe::Cache::sprite2d->setUniform<qe::UNIORIGIN>(origin);
            qe::Cache::sprite2d->setUniform<qe::UNISIZE>(size);
            // set texture
            qe::Cache::buttont->bindTo();
            // set vao
            qe::Cache::meshm->render();
        }
        void render_button(ui::Point a, ui::Point b) {
            glm::vec2 origin = glm::vec2(a.x, a.y) * glm::vec2(2, 2) / _ctxt->getResolution();
            glm::vec2 dimension = glm::vec2(b.x, b.y) * glm::vec2(2, 2) / _ctxt->getResolution();
            render_button(origin - glm::vec2(1, 1), dimension);
        }
        void render_background() {
            // TODO
        }
        void process_events() {
            _ctxt->waitEvents();
        }
        virtual void mouse_button_callback(int, int, int) {
            glm::ivec2 cpos = _ctxt->getMousePos();
            _ui->click(ui::Point {(float)cpos.x, (float)(_ctxt->getResolution().y - cpos.y)});
        }
        void key_callback(int, int) {}
        void mouse_callback(double, double) {}
        void idle_callback() {}
        void load_ui() {
            _ui.reset(new ui::UI(ui::Point {(float)_ctxt->width(), (float)_ctxt->height()}));
            std::unique_ptr<ui::Box> box(new ui::Box());
            std::unique_ptr<ui::Text> text1(new ui::Text());
            std::unique_ptr<ui::Text> text2(new ui::Text());

            text1->dimension() = ui::Point {0.15, 0.15};
            text2->dimension() = ui::Point {0.15, 0.15};
            text1->margin() = ui::Point {0.02, 0.02};
            text2->margin() = ui::Point {0.02, 0.02};
            text1->padding() = ui::Point {0.01, 0.01};
            text2->padding() = ui::Point {0.01, 0.01};

            text1->text() = "Start Game";
            text2->text() = "Quit";

            box->orientation() = ui::Box::Orientation::VERTICAL;
            box->expand() = true;
            box->align_x() = ui::Box::Align::CENTER;
            box->align_y() = ui::Box::Align::CENTER;

            box->append(text2.release());
            box->append(text1.release());
            box->convert_coords(_ui->res());
            box->origin() = ui::Point {0, 0};
            box->dimension() = _ui->res();
            _ui->add_layer("mainmenu", box.release());

            auto *quit = _ui->get(0, "1.2");
            auto *start = _ui->get(0, "1.1");
            start->on_click([this](void*) mutable {
                _gamescreen->newGame(glm::ivec2(20, 14),
                    gamespace::Player(glm::vec3(0.448, 0.884, 1), "Blue"),
                    gamespace::Player(glm::vec3(1, 0.448, 0.448), "Red"));
                _manager->changeActiveScreen(*_gamescreen);
            });
            quit->on_click([this](void*) mutable {
                _manager->quit();
            });
            auto renderer = [this](ui::Renderable *t) mutable {
                render_rectangle(t->origin() + t->margin(), t->dimension() - t->margin(), glm::vec3(0.3, 0.3, 0.3), _ctxt->getResolution());
                render_text(t, glm::vec3(1, 1, 1));
            };
            start->render_with(renderer);
            quit->render_with(renderer);
            auto d = [](void *t){delete (text_t*)t;};
            start->payload(d);
            quit->payload(d);
            start->payload() = nullptr;
            quit->payload() = nullptr;
        }
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "MainScreen" << std::endl;
            _ui->__introspect(off + 2);
        }
    };
}

#endif
