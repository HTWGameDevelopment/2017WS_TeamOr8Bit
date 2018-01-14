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
        void key_callback(int key, int action) {}
        void mouse_callback(double x, double y) {}
        void mouse_button_callback(int button, int action, int mods) {}
        void idle_callback() {}
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "AboutScreen" << std::endl;
        }
    };
}

#endif
