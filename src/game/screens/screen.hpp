#ifndef GAMESPACE_MENU
#define GAMESPACE_MENU

namespace gamespace {

    class Screen {
    public:
        virtual void activate_screen() = 0;
        virtual void deactivate_screen() = 0;
        void key_callback(int key, int, int action, int) {}
        void mouse_callback(double x, double y) {}
        void mouse_button_callback(int button, int action, int mods) {}
        void idle_callback() {}
    };

}

#endif
