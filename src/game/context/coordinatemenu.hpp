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
        BoardTile *_b;
        Unit *_u;
        void update();
        void registerCallbacks();
        void destroy();
    public:
        static CoordinateMenu *createForTile(BoardTile *b, ui::UI *dui, glm::vec2 res) {
            std::unique_ptr<ui::Box> contextui(new ui::Box());
            std::unique_ptr<ui::Box> cb11(new ui::Box());
            std::unique_ptr<ui::Box> cb2(new ui::Box());
            std::unique_ptr<ui::Text> ct1(new ui::Text());
            std::unique_ptr<ui::Text> ct2(new ui::Text());
            std::unique_ptr<ui::Text> ct3(new ui::Text());
            std::unique_ptr<ui::Text> ct4(new ui::Text());

            contextui->dimension() = ui::Point {0.075, 0.06};
            ct1->dimension() = ui::Point {0.025, 0.03};
            ct2->dimension() = ui::Point {0.045, 0.03};
            ct3->dimension() = ui::Point {0.025, 0.03};
            ct4->dimension() = ui::Point {0.045, 0.03};
            ct1->margin() = ui::Point {0.005, 0.005};
            ct2->margin() = ui::Point {0.005, 0.005};
            ct3->margin() = ui::Point {0.005, 0.005};
            ct4->margin() = ui::Point {0.005, 0.005};
            contextui->orientation() = ui::Box::VERTICAL;
            cb11->orientation() = ui::Box::HORIZONTAL;
            cb2->orientation() = ui::Box::HORIZONTAL;
            contextui->expand() = false;
            cb11->expand() = false;
            cb2->expand() = false;
            contextui->align_x() = ui::Box::BEGINNING;
            contextui->align_y() = ui::Box::BEGINNING;
            cb11->align_x() = ui::Box::BEGINNING;
            cb11->align_y() = ui::Box::BEGINNING;
            cb2->align_x() = ui::Box::BEGINNING;
            cb2->align_y() = ui::Box::BEGINNING;

            cb11->append(ct1.release());
            cb11->append(ct2.release());
            cb2->append(ct3.release());
            cb2->append(ct4.release());
            contextui->append(cb11.release());
            contextui->append(cb2.release());
            auto *m = new CoordinateMenu(b, dui);
            contextui->payload() = m;
            m->init(contextui.get());
            contextui->payload([](void *t){delete (CoordinateMenu*)t;});
            contextui->render_with([res](ui::Renderable *t) mutable {
                render_rectangle(t->origin(), t->dimension(), glm::vec3(0.8, 0.8, 0.8), res);
            });
            auto text_renderer = [](ui::Renderable *t) mutable {
                if(t->payload() == nullptr) {
                    t->payload() = new text_t(
                        ((ui::Text*)t)->text(),
                        qe::Cache::glyphlatin,
                        // to_ivec2(t->origin() + t->margin() + t->padding() + ui::absp_t {0, 0.5} * (t->dimension() - t->margin() - t->padding())),
                        to_ivec2(t->origin() + t->margin() + t->padding()),
                        (int)(0.5 * (t->dimension().y - t->margin().y - t->padding().y)),
                        (int)(t->dimension().x - t->margin().x - t->padding().x));
                }
                text_t *pl = (text_t*)t->payload();
                pl->foreground() = glm::vec3(0, 0, 0);
                pl->render();
            };
            m->set_renderer_payload(text_renderer, [](void *t){delete (text_t*)t;});
            contextui->convert_coords(dui->res());
            dui->add_context_menu(contextui.release());
            return m;
        }
        CoordinateMenu(BoardTile *b, ui::UI *ui);
        virtual ~CoordinateMenu() {}
        ui::Renderable *view() {
            return _ui;
        }
        ui::Renderable *titlebar() {
            return _titlebar;
        }
        void init(ui::Renderable *ui) {
            _ui = ui;
            _titlebar = ui->get("1");
            _coordinates = (ui::Text*)ui->get("1.1");
            _unit_text = (ui::Text*)ui->get("1.2");
            _hp_text = (ui::Text*)ui->get("2.1");
            _hp_text->text() = "HP";
            _unit_hp = (ui::Text*)ui->get("2.2");
            update();
        }
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
            _coordinates->payload(payload);
            _unit_text->payload(payload);
            _unit_hp->payload(payload);
            _hp_text->payload(payload);
        }
    };

}

#endif
