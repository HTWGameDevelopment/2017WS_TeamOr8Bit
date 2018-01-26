#include "coordinatemenu.hpp"

#include<game/board.hpp>
#include<game/unit.hpp>

using namespace gamespace;

CoordinateMenu::CoordinateMenu(BoardTile *b, ui::UI *ui): _top(ui), _b(b), _u(b->unit()) {
    assert(_u);
    _udisp = _u->on_change([this](){
        if(_u->dead()) {
            destroy();
        } else {
            update();
        }
    });
}

CoordinateMenu::~CoordinateMenu() {
    _u->on_change_r(_udisp);
}

void CoordinateMenu::destroy() {
    _top->hide_context_menu(_ui);
}

void rebuild_payload(ui::Renderable *t) {
    assert(t->root());
    t->origin() += t->root()->origin();
    t->payload() = new text_t(
        ((ui::Text*)t)->text(),
        qe::Cache::glyphlatin,
        to_ivec2(t->origin() + t->margin() + t->padding() + ui::Point {0, 0.25} * (t->dimension() - t->margin() - t->padding())),
        //to_ivec2(t->origin() + t->margin() + t->padding()),
        (int)(0.5 * (t->dimension().y - t->margin().y - t->padding().y)),
        (int)(t->dimension().x - t->margin().x - t->padding().x));
    auto i = ((text_t*)t->payload())->recalculate_dimension();
    auto pi = ui::Point {i.x, i.y};
    t->dimension().x = std::max(t->dimension().x, pi.x);
    t->dimension().y = std::max(t->dimension().y, pi.y);
    t->origin() -= t->root()->origin();
}

void CoordinateMenu::update() {
    assert(_u);
    _b = _u->tile();
    _coordinates->text() = "[" + std::to_string(_b->coord().x) + ":" + std::to_string(_b->coord().y) + "]";
    _unit_text->text() = _u->name();
    _unit_hp->text() = std::to_string(_u->hp()) + "/" + std::to_string(_u->max_hp());
    invalidate();
    auto osave = _ui->origin();
    _ui->origin() = ui::Point {0, 0};
    _ui->recalculate_dimension();
    _ui->recalculate_origin();
    _ui->origin() = osave;
    invalidate();
    // TODO find a way around double invalidate
}

void CoordinateMenu::invalidate() {
    _coordinates->delete_payload();
    _unit_text->delete_payload();
    _hp_text->delete_payload();
    _qt->delete_payload();
    _unit_hp->delete_payload();
    rebuild_payload(_hp_text);
    rebuild_payload(_coordinates);
    rebuild_payload(_unit_text);
    rebuild_payload(_unit_hp);
    rebuild_payload(_qt);
}
