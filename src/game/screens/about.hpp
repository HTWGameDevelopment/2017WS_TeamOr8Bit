#ifndef GAMESPACE_ABOUTMENU
#define GAMESPACE_ABOUTMENU

#include<iostream>
#include<game/screens/screen.hpp>

namespace gamespace {
    class AboutScreen: public Screen {
public:
        void activate_screen() {

        }
        void deactivate_screen() {

        }
        void key_callback(int, int) {}
        void mouse_callback(double, double) {}
        void mouse_button_callback(int, int, int) {}
        void idle_callback() {}
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "AboutScreen" << std::endl;
        }
    };
}

#endif
