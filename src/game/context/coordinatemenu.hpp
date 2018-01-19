#ifndef GAMESPACE_COORDINATEMENU_HPP
#define GAMESPACE_COORDINATEMENU_HPP

#include<engine/cache.hpp>
#include<engine/text.hpp>

#include<ui/contextui.hpp>
#include<ui/definedui.hpp>
#include<ui/abstracttext.hpp>
#include<ui/abstractbox.hpp>
#include<ui/definedtext.hpp>

inline void render_button(glm::vec2 origin, glm::vec2 size, glm::vec3 bg) {
    qe::Cache::sprite2dcolor->use();
    // set shader
    qe::Cache::sprite2dcolor->setUniform<qe::UNIORIGIN>(origin);
    qe::Cache::sprite2dcolor->setUniform<qe::UNISIZE>(size);
    qe::Cache::sprite2dcolor->setUniform<qe::UNITEXTBG>(bg);
    // set vao
    qe::Cache::meshm->render();
}

inline void render_rectangle(ui::defp_t a, ui::defp_t b, glm::vec3 bg, glm::vec2 res) {
    glm::vec2 origin = glm::vec2(a.x, a.y) * glm::vec2(2, 2) / res;
    glm::vec2 dimension = glm::vec2(b.x, b.y) * glm::vec2(2, 2) / res;
    render_button(origin - glm::vec2(1, 1), dimension, bg);
}

inline glm::ivec2 to_ivec2(ui::defp_t t) {
    return glm::ivec2(t.x, t.y);
}

namespace gamespace {

    typedef decltype(qe::Cache::glyphlatin) glyphmap;
    typedef qe::Text<std::remove_pointer<glyphmap>::type> text_t;

    class BoardTile;
    class Unit;

    class CoordinateMenu: public ui::ContextUIModel {
    private:
        ui::DefinedUI *_top;
        ui::DefinedContextUI *_ui;
        ui::DefinedRenderable *_titlebar;
        ui::DefinedText *_coordinates;
        ui::DefinedText *_unit_text;
        ui::DefinedText *_hp_text;
        ui::DefinedText *_unit_hp;
        BoardTile *_b;
        Unit *_u;
        void update();
        void registerCallbacks();
        void destroy();
    public:
        static CoordinateMenu *createForTile(BoardTile *b, ui::DefinedUI *dui, glm::vec2 res) {
            std::unique_ptr<ui::AbstractContextUI> contextui(new ui::AbstractContextUI());
            std::unique_ptr<ui::AbstractBox> cb1(new ui::AbstractBox());
            std::unique_ptr<ui::AbstractBox> cb11(new ui::AbstractBox());
            std::unique_ptr<ui::AbstractBox> cb2(new ui::AbstractBox());
            std::unique_ptr<ui::AbstractText> ct1(new ui::AbstractText());
            std::unique_ptr<ui::AbstractText> ct2(new ui::AbstractText());
            std::unique_ptr<ui::AbstractText> ct3(new ui::AbstractText());
            std::unique_ptr<ui::AbstractText> ct4(new ui::AbstractText());

            contextui->dimension() = ui::absp_t {0.65, 0.6};
            ct1->dimension() = ui::absp_t {0.025, 0.03};
            ct2->dimension() = ui::absp_t {0.045, 0.03};
            ct3->dimension() = ui::absp_t {0.025, 0.03};
            ct4->dimension() = ui::absp_t {0.045, 0.03};
            ct1->margin() = ui::absp_t {0.005, 0.005};
            ct2->margin() = ui::absp_t {0.005, 0.005};
            ct3->margin() = ui::absp_t {0.005, 0.005};
            ct4->margin() = ui::absp_t {0.005, 0.005};
            cb1->set_orientation(ui::AbstractBox::VERTICAL);
            cb11->set_orientation(ui::AbstractBox::HORIZONTAL);
            cb2->set_orientation(ui::AbstractBox::HORIZONTAL);
            cb1->set_growth(ui::AbstractBox::MINIMUM);
            cb11->set_growth(ui::AbstractBox::MINIMUM);
            cb2->set_growth(ui::AbstractBox::MINIMUM);
            cb1->set_align_inner(ui::AbstractBox::BEGINNING, ui::AbstractBox::BEGINNING);
            cb11->set_align_inner(ui::AbstractBox::BEGINNING, ui::AbstractBox::BEGINNING);
            cb2->set_align_inner(ui::AbstractBox::BEGINNING, ui::AbstractBox::BEGINNING);

            cb11->append(ct2.release());
            cb11->append(ct1.release());
            cb2->append(ct4.release());
            cb2->append(ct3.release());
            cb1->append(cb11.release());
            cb1->append(cb2.release());
            contextui->set_inner(cb1.release());
            auto *c = contextui->buildDefined(dui->resolution());
            auto *m = new CoordinateMenu(b);
            c->set_ui(dui);
            c->set_model(m);
            c->get_inner()->render_with([res](ui::DefinedRenderable *t) mutable {
                render_rectangle(t->origin(), t->dimension(), glm::vec3(0.8, 0.8, 0.8), res);
            });
            auto text_renderer = [](ui::DefinedRenderable *t) mutable {
                if(t->payload() == nullptr) {
                    t->payload() = new text_t(
                        ((ui::DefinedText*)t)->text(),
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
            dui->add_context_menu(c);
            return m;
        }
        CoordinateMenu(BoardTile *b);
        virtual ~CoordinateMenu() {}
        ui::DefinedContextUI *view() {
            return _ui;
        }
        ui::DefinedRenderable *titlebar() {
            return _titlebar;
        }
        void init(ui::DefinedContextUI *ui) {
            _ui = ui;
            _titlebar = ui->get("1");
            _coordinates = (ui::DefinedText*)ui->get("1.1");
            _unit_text = (ui::DefinedText*)ui->get("1.2");
            _hp_text = (ui::DefinedText*)ui->get("2.1");
            _hp_text->text() = "HP";
            _unit_hp = (ui::DefinedText*)ui->get("2.2");
            update();
        }
        virtual void invalidate();
        void show() {
            _ui->show();
        }
        void hide() {
            _ui->hide();
        }
        ui::DefinedText *coordinates() {
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
