#ifndef GAMESPACE_COORDINATEMENU_HPP
#define GAMESPACE_COORDINATEMENU_HPP

#include<engine/cache.hpp>
#include<engine/text.hpp>

#include<ui/ui.hpp>
#include<ui/text.hpp>
#include<ui/box.hpp>

namespace gamespace {

    class BoardTile;
    class Unit;
    class Match;

    class CoordinateMenu {
    private:
        ui::UI *_top;
        ui::Renderable *_ui;
        ui::Renderable *_titlebar;
        ui::Text *_coordinates;
        ui::Text *_unit_text;
        ui::Text *_hp_text;
        ui::Text *_unit_hp;
        ui::Text *_qt;
        BoardTile *_b;
        Unit *_u;
        std::vector<ui::Renderable*> _action_items;
        unsigned int _udisp;
        unsigned int _mdisp;
        Match *_match;
        void update();
        void registerCallbacks();
        void destroy();
    public:
        static CoordinateMenu *createForTile(BoardTile *b, ui::UI *dui, glm::vec2 res);
        CoordinateMenu(BoardTile *b, ui::UI *ui);
        virtual ~CoordinateMenu();
        ui::Renderable *view() {
            return _ui;
        }
        ui::Renderable *titlebar() {
            return _titlebar;
        }
        void init(ui::Renderable *ui);
        virtual void invalidate();
        void show() {
            _ui->show();
        }
        void hide() {
            _ui->hide();
        }
        ui::Text *coordinates() {
            return _coordinates;
        }
        template<typename F, typename D>
        void set_renderer_payload(F &&renderer, D &&payload) {
            _coordinates->render_with(renderer);
            _unit_text->render_with(renderer);
            _unit_hp->render_with(renderer);
            _hp_text->render_with(renderer);
            _qt->render_with(renderer);
            _coordinates->payload(payload);
            _unit_text->payload(payload);
            _unit_hp->payload(payload);
            _hp_text->payload(payload);
            _qt->payload(payload);
        }
        Unit *unit() {
            return _u;
        }
    };

}

#endif
