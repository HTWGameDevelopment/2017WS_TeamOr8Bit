#ifndef GAMESPACE_ENDINGMENU
#define GAMESPACE_ENDINGMENU

#include<engine/cache.hpp>
#include<engine/text.hpp>
#include<engine/context.hpp>

#include<iostream>
#include<screenmanager/screen.hpp>
#include<game/player.hpp>
#include<game/screens/screen.hpp>
#include<ui/text.hpp>
#include<ui/box.hpp>
#include<ui/ui.hpp>

namespace gamespace {

    typedef decltype(qe::Cache::glyphlatin) glyphmap;
    typedef qe::Text<std::remove_pointer<glyphmap>::type> text_t;

    class MainScreen;

    class EndingScreen: public Screen {
    private:
        MainScreen *_main;
        qe::Context *_ctxt;
        glyphmap _glyphmap;
        screen::ScreenManager<Screen> *_manager;
        bool _active;
        std::unique_ptr<ui::UI> _ui;
        Player *_player;
        float _resy;
        float _last_x;
        float _last_y;
    public:
        EndingScreen(screen::ScreenManager<Screen> &manager, qe::Context *ctxt, glyphmap glyphmap)
        : Screen(), _main(nullptr), _ctxt(ctxt), _glyphmap(glyphmap), _manager(&manager), _player(nullptr) {
            assert(ctxt);
            load_ui();
            _resy = ctxt->getResolution().y;
        }
        void setWinningPlayer(Player *player) {
            _player = player;
        }
        void activate_screen() {
            assert(_player);
            GDBG("activating ending screen");
            ((ui::Text*)_ui->get(0, "1.1"))->text() = "Congratulations " + _player->name();
            _active = true;
            _ctxt->displayCursor(_ctxt->getCenterCoordinate());
            _ctxt->textcontext();
            while(_active) {
                _ctxt->start();
                _ui->render();
                _ctxt->swap();
                _ctxt->waitEvents();
            }
            _manager->changeActiveScreen(_manager->getScreen(2));
        }
        void deactivate_screen() {
            _active = false;
            GDBG("deactivating ending screen");
        }
        void linkMain(MainScreen *main) {
            _main = main;
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
        void key_callback(int, int) {}
        void mouse_callback(double x, double y) {
            _last_x = x;
            _last_y = y;
        }
        void mouse_button_callback(int button, int action, int) {
            if(action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
                ui::Point s{_last_x, _resy - _last_y};
                _ui->click(s);
            }
    }
        void idle_callback() {}
        void load_ui();
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "EndingScreen" << std::endl;
        }
    };
}

#endif
