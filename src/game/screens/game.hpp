#ifndef GAMESPACE_GAMEMENU
#define GAMESPACE_GAMEMENU

#include<game/screens/ending.hpp>
#include<game/screens/game_impl.hpp>
#include<game/screens/gameinput.hpp>
#include<game/screens/screen.hpp>

#include<screenmanager/screen.hpp>

namespace gamespace {
    class GameScreen: public Screen {
    private:
        std::unique_ptr<GameScreenImpl> _impl;
        qe::Context *_ctxt;
        std::shared_ptr<font::Font> _font;
        std::unique_ptr<GameScreenInputState> _input;
        screen::ScreenManager<Screen> *_manager;
        bool _block_input;
        bool _goto_ending = false;
    public:
        GameScreen(screen::ScreenManager<Screen> &manager, qe::Context &ctxt, std::shared_ptr<font::Font> font): _ctxt(&ctxt), _font(font), _manager(&manager), _block_input(true) {}
        void newGame(glm::ivec2 size, gamespace::Player player1, gamespace::Player player2) {
            _impl.reset(new GameScreenImpl(gamespace::Match(size, player1, player2), _ctxt, _font));
            _input.reset(new GameScreenInputState(*_impl.get()));
            _goto_ending = false;
            _impl->match().on_win([this](Player &player){
                GDBG("win triggered");
                ((EndingScreen*)&_manager->getScreen(3))->setWinningPlayer(&player);
                _goto_ending = true;
                _impl->deactivate();
            });
        }
        void activate_screen() {
            assert(_impl.get() && _input.get());
            _block_input = true;
            _impl->pre_run();
            _block_input = false;
            _impl->run();
            if(_goto_ending) _manager->changeActiveScreen(_manager->getScreen(3));
            else _manager->changeActiveScreen(_manager->getScreen(2));
        }
        void deactivate_screen() {
            assert(_impl.get() && _input.get());
            GDBG("deactivating game screen");
            _block_input = true;
            _impl->deactivate();
        }
        virtual void key_callback(int key, int action) {
            assert(_impl.get() && _input.get());
            if(_block_input) return;
            _input->key(key, action);
        }
        virtual void mouse_callback(double x, double y) {
            assert(_impl.get() && _input.get());
            if(_block_input) return;
            _input->mouse(x, y);
        }
        virtual void mouse_button_callback(int button, int action, int mods) {
            assert(_impl.get() && _input.get());
            if(_block_input) return;
            _input->button(button, action, mods);
        }
        virtual void idle_callback() {
            assert(_impl.get() && _input.get());
            if(_block_input) return;
            _input->idle();
        }
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "GameScreen" << std::endl;
            if(_impl.get()) _impl->__introspect(off + 2);
        }
    };
}

#endif
