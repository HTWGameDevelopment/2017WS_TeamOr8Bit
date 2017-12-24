#ifndef GAMESPACE_MENU
#define GAMESPACE_MENU

namespace gamespace {

    class Screen {
    public:
        virtual void activate_screen() = 0;
        virtual void deactivate_screen() = 0;
    };

}

#endif
