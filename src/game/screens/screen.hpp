#ifndef GAMESPACE_MENU
#define GAMESPACE_MENU

#include<string.h>

namespace gamespace {

    class Screen {
    public:
        virtual ~Screen() {}
        virtual void activate_screen() = 0;
        virtual void deactivate_screen() = 0;
        virtual void key_callback(int key, int action) = 0;
        virtual void mouse_callback(double x, double y) = 0;
        virtual void mouse_button_callback(int button, int action, int mods) = 0;
        virtual void idle_callback() = 0;
        virtual void __introspect(size_t off) = 0;
    };

}

#endif
