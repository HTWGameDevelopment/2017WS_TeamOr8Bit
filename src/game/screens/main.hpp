#ifndef GAMESPACE_MAINMENU
#define GAMESPACE_MAINMENU

#include<engine/cache.hpp>
#include<engine/context.hpp>
#include<engine/text.hpp>

#include<game/screens/screen.hpp>
#include<game/screens/about.hpp>
#include<game/screens/game.hpp>

#include<ui/abstractbox.hpp>
#include<ui/abstracttext.hpp>
#include<ui/definedtext.hpp>
#include<ui/uifactory.hpp>
#include<screenmanager/screen.hpp>

inline glm::ivec2 to_ivec2(ui::defp_t t) {
    return glm::ivec2(t.x, t.y);
}

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
        std::unique_ptr<ui::DefinedUI> _ui;
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
        void render_button(ui::defp_t a, ui::defp_t b) {
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
            text1->padding() = ui::absp_t {0.01, 0.01};
            text2->padding() = ui::absp_t {0.01, 0.01};

            text1->text() = "Start Game";
            text2->text() = "Quit";

            box->set_orientation(ui::AbstractBox::Orientation::VERTICAL);
            box->set_growth(ui::AbstractBox::Growth::FILL);
            box->set_align_inner(ui::AbstractBox::Align::CENTER, ui::AbstractBox::Align::CENTER);

            box->append(text1.release());
            box->append(text2.release());
            ui.set_container(box.release());

            _ui.reset(new ui::DefinedUI(ui::UIFactory(ui, _ctxt->width(), _ctxt->height()).release()));
            auto *quit = _ui->get("1.2");
            auto *start = _ui->get("1.1");
            start->on_click([this](void*) mutable {
                _gamescreen->newGame(glm::ivec2(20, 14),
                    gamespace::Player(glm::vec3(0.448, 0.884, 1), "Blue"),
                    gamespace::Player(glm::vec3(1, 0.448, 0.448), "Red"));
                _manager->changeActiveScreen(*_gamescreen);
            });
            quit->on_click([this](void*) mutable {
                _manager->quit();
            });
            auto renderer = [this](ui::DefinedRenderable *t) mutable {
                this->render_button(t->origin() + t->margin(), t->dimension() - t->margin());
                if(t->payload() == nullptr) {
                    t->payload() = new text_t(
                        ((ui::DefinedText*)t)->text(),
                        _glyphmap,
                        to_ivec2(t->origin() + t->margin() + t->padding() + ui::absp_t {0, 0.5} * (t->dimension() - t->margin() - t->padding())),
                        (int)(0.5 * (t->dimension().y - t->margin().y - t->padding().y)),
                        (int)(t->dimension().x - t->margin().x - t->padding().x));
                }
                ((text_t*)(t->payload()))->render();
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
