#include "coordinatemenu.hpp"

#include<game/board.hpp>
#include<game/unit.hpp>

using namespace gamespace;
CoordinateMenu *CoordinateMenu::createForTile(BoardTile *b, ui::UI *dui, glm::vec2 res) {
    std::unique_ptr<ui::Box> contextui(new ui::Box());
    std::unique_ptr<ui::Box> cb11(new ui::Box());
    std::unique_ptr<ui::Box> cb2(new ui::Box());
    std::unique_ptr<ui::Box> cba(new ui::Box()); // action box
    std::unique_ptr<ui::Text> ct1(new ui::Text());
    std::unique_ptr<ui::Text> ct2(new ui::Text());
    std::unique_ptr<ui::Text> ctq(new ui::Text());
    std::unique_ptr<ui::Text> ct3(new ui::Text());
    std::unique_ptr<ui::Text> ct4(new ui::Text());

    contextui->dimension() = ui::Point {0.085, 0.06};
    ct1->dimension() = ui::Point {0.025, 0.03};
    ct2->dimension() = ui::Point {0.045, 0.03};
    ctq->dimension() = ui::Point {0.015, 0.03};
    ct3->dimension() = ui::Point {0.025, 0.03};
    ct4->dimension() = ui::Point {0.045, 0.03};
    ct1->margin() = ui::Point {0.005, 0.005};
    ct2->margin() = ui::Point {0.005, 0.005};
    ctq->margin() = ui::Point {0.005, 0.005};
    ct3->margin() = ui::Point {0.005, 0.005};
    ct4->margin() = ui::Point {0.005, 0.005};
    contextui->orientation() = ui::Box::VERTICAL;
    cb11->orientation() = ui::Box::HORIZONTAL;
    cb2->orientation() = ui::Box::HORIZONTAL;
    cba->orientation() = ui::Box::VERTICAL;
    contextui->expand() = false;
    cb11->expand() = false;
    cb2->expand() = false;
    cba->expand() = false;
    contextui->align_x() = ui::Box::BEGINNING;
    contextui->align_y() = ui::Box::BEGINNING;
    cb11->align_x() = ui::Box::BEGINNING;
    cb11->align_y() = ui::Box::BEGINNING;
    cb2->align_x() = ui::Box::BEGINNING;
    cb2->align_y() = ui::Box::BEGINNING;
    cba->align_x() = ui::Box::CENTER;
    cba->align_y() = ui::Box::BEGINNING;

    cb11->append(ct1.release());
    cb11->append(ct2.release());
    cb11->append(ctq.release());
    cb2->append(ct3.release());
    cb2->append(ct4.release());
    contextui->append(cba.release());
    contextui->append(cb2.release());
    contextui->append(cb11.release());
    contextui->origin() = ui::Point {0, 0};
    auto *m = new CoordinateMenu(b, dui);
    m->init(contextui.get());
    contextui->payload() = m;
    contextui->payload([](void *t){delete (CoordinateMenu*)t;});
    contextui->render_with([res](ui::Renderable *t) mutable {
        render_rectangle(t->origin(), t->dimension(), glm::vec3(0.8, 0.8, 0.8), res);
    });
    contextui->get("1")->render_with([res](ui::Renderable *t) mutable {
        render_rectangle(t->origin(), t->dimension(), glm::vec3(0.5, 0.5, 0.5), res);
    });
    auto text_renderer = [](ui::Renderable *t) mutable {
        assert(t->payload());
        text_t *pl = (text_t*)t->payload();
        pl->foreground() = glm::vec3(0, 0, 0);
        pl->render();
    };
    m->set_renderer_payload(text_renderer, [](void *t){delete (text_t*)t;});
    contextui->convert_coords(dui->res());
    dui->add_context_menu(contextui.release());
    m->update();
    return m;
}

CoordinateMenu::CoordinateMenu(BoardTile *b, ui::UI *ui): _top(ui), _b(b), _u(b->unit()) {
    assert(_u);
    _udisp = _u->on_change([this](bool alive){
        if(alive == false) {
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

void CoordinateMenu::init(ui::Renderable *ui) {
    _ui = ui;
    ui->set_root(ui);
    _titlebar = ui->get("1");
    _coordinates = (ui::Text*)ui->get("1.1");
    _unit_text = (ui::Text*)ui->get("1.2");
    _qt = (ui::Text*)ui->get("1.3");
    _qt->text() = "X";
    _qt->on_click([this](ui::Renderable* t) {
        destroy();
    });
    _hp_text = (ui::Text*)ui->get("2.1");
    _hp_text->text() = "HP";
    _unit_hp = (ui::Text*)ui->get("2.2");
    // actions
    auto *cont = (ui::Box*)ui->get("3");
    auto &c = _u->getSpecialActions();
    for(auto b = c.begin(); b != c.end(); ++b) {
        std::unique_ptr<ui::Text> t(new ui::Text());
        t->dimension() = ui::Point {0.045, 0.03};
        t->margin() = ui::Point {0.005, 0.005};
        t->text() = b->name;
        t->set_root(ui);
        auto text_renderer = [](ui::Renderable *t) mutable {
            assert(t->payload());
            text_t *pl = (text_t*)t->payload();
            pl->foreground() = glm::vec3(0, 0, 0);
            pl->render();
        };
        t->render_with(text_renderer);
        t->payload([](void *t){delete (text_t*)t;});
        t->on_click([this, f = b->f](ui::Renderable*){f(_u);});
        _action_items.push_back(t.get());
        cont->append(t.release());
    }
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
    for(auto *r : _action_items) {
        r->delete_payload();
        rebuild_payload(r);
    }
}
