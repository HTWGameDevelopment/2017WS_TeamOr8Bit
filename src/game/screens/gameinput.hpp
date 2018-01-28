#ifndef GAMESPACE_GAMESCREEN_INPUT_HPP
#define GAMESPACE_GAMESCREEN_INPUT_HPP

#include<game/screens/game_impl.hpp>

namespace gamespace {

    const unsigned int MOVEUP = 0;
    const unsigned int MOVEDOWN = 1;
    const unsigned int MOVELEFT = 2;
    const unsigned int MOVERIGHT = 3;
    const unsigned int MOVEFORWARD = 4;
    const unsigned int MOVEBACKWARD = 5;

    class GameScreenInputState {
        bool _movementmask[6];
        enum movement_mode { LOCKED, FREE, UIINTERACTION };
        double _last_x;
        double _last_y;
        ui::Point _origin_save;
        ui::Point _mouse_save;
        movement_mode _mouse_mode;
        double _resy;
        ui::Renderable *_selected_menu;
        GameScreenImpl *_impl;
    public:
        GameScreenInputState(GameScreenImpl &impl);
        void key(int key, int action);
        void mouse(double x, double y);
        void button(int button, int action, int mods);
        void idle();
    };
}

#endif
