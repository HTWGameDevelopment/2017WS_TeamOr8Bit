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

void CoordinateMenu::update() {
    assert(_u);
    _b = _u->tile();
    _coordinates->text() = "[" + std::to_string(_b->coord().x) + ":" + std::to_string(_b->coord().y) + "]";
    _coordinates->delete_payload();
    _unit_text->text() = _u->name();
    _unit_hp->text() = std::to_string(_u->hp()) + "/" + std::to_string(_u->max_hp());
    _unit_text->delete_payload();
    _unit_hp->delete_payload();
}

void CoordinateMenu::invalidate() {
    _coordinates->delete_payload();
    _unit_text->delete_payload();
    _hp_text->delete_payload();
    _qt->delete_payload();
    _unit_hp->delete_payload();
}
