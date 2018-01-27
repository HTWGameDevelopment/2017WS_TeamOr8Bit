#ifndef GAMESPACE_COORDINATEMENU_HPP
#define GAMESPACE_COORDINATEMENU_HPP

#include<engine/cache.hpp>
#include<engine/text.hpp>

#include<ui/ui.hpp>
#include<ui/text.hpp>
#include<ui/box.hpp>

inline void render_button(glm::vec2 origin, glm::vec2 size, glm::vec3 bg) {
    qe::Cache::sprite2dcolor->use();
    // set shader
    qe::Cache::sprite2dcolor->setUniform<qe::UNIORIGIN>(origin);
    qe::Cache::sprite2dcolor->setUniform<qe::UNISIZE>(size);
    qe::Cache::sprite2dcolor->setUniform<qe::UNITEXTBG>(bg);
    // set vao
    qe::Cache::meshm->render();
}

inline void render_rectangle(ui::Point a, ui::Point b, glm::vec3 bg, glm::vec2 res) {
    glm::vec2 origin = glm::vec2(a.x, a.y) * glm::vec2(2, 2) / res;
    glm::vec2 dimension = glm::vec2(b.x, b.y) * glm::vec2(2, 2) / res;
    render_button(origin - glm::vec2(1, 1), dimension, bg);
}

inline glm::ivec2 to_ivec2(ui::Point t) {
    return glm::ivec2(t.x, t.y);
}

namespace gamespace {

    typedef decltype(qe::Cache::glyphlatin) glyphmap;
    typedef qe::Text<std::remove_pointer<glyphmap>::type> text_t;

    class BoardTile;
    class Unit;

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
