#ifndef GAMESPACE_COORDINATEMENU_HPP
#define GAMESPACE_COORDINATEMENU_HPP

#include<game/unit.hpp>

#include<ui/contextui.hpp>
#include<ui/definedui.hpp>
#include<ui/definedtext.hpp>

namespace gamespace {

    class CoordinateMenu: public ui::ContextUIModel {
    private:
        ui::DefinedContextUI *_ui;
        ui::DefinedText *_coordinates;
        ui::DefinedText *_unit_text;
        ui::DefinedText *_hp_text;
        ui::DefinedText *_unit_hp;
        Unit *_last_u;
    public:
        void init(ui::DefinedContextUI *ui) {
            _ui = ui;
            _coordinates = (ui::DefinedText*)ui->get("1.1");
            _unit_text = (ui::DefinedText*)ui->get("1.2");
            _hp_text = (ui::DefinedText*)ui->get("2.1");
            _hp_text->text() = "HP";
            _unit_hp = (ui::DefinedText*)ui->get("2.2");
            _last_u = nullptr;
        }
        void set_unit(Unit *u) {
            if(_last_u == u) return;
            _last_u = u;
            if(u) {
                _unit_text->text() = u->name();
                _unit_hp->text() = std::to_string(u->hp()) + "/" + std::to_string(u->max_hp());
            } else {
                _unit_text->text() = "";
                _unit_hp->text() = "";
            }
            _unit_text->delete_payload();
            _unit_hp->delete_payload();
        }
        void set_coords(int x, int y) {
            _coordinates->text() = "[" + std::to_string(x) + ":" + std::to_string(y) + "]";
            _coordinates->delete_payload();
        }
        void on_show() {

        }
        void on_hide() {

        }
        void show(glm::ivec2 center) {
            _ui->origin() = ui::defp_t {center.x, center.y};
            _ui->ui()->show_context_menu(_ui);
        }
        void hide() {
            _ui->ui()->hide_context_menu(_ui);
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
