#include "ending.hpp"

#include<game/screens/main.hpp>
#include<game/uirender.hpp>

using namespace gamespace;

void EndingScreen::load_ui() {
    _ui.reset(new ui::UI(ui::Point {(float)_ctxt->width(), (float)_ctxt->height()}));
    std::unique_ptr<ui::Box> box(new ui::Box());
    std::unique_ptr<ui::Text> text1(new ui::Text());
    std::unique_ptr<ui::Text> text2(new ui::Text());

    text1->dimension() = ui::Point {0.15, 0.15};
    text2->dimension() = ui::Point {0.15, 0.15};
    text1->margin() = ui::Point {0.02, 0.02};
    text2->margin() = ui::Point {0.02, 0.02};
    text1->padding() = ui::Point {0.01, 0.01};
    text2->padding() = ui::Point {0.01, 0.01};

    text1->text() = "Congratulations";
    text2->text() = "Return";

    box->orientation() = ui::Box::Orientation::VERTICAL;
    box->expand() = true;
    box->align_x() = ui::Box::Align::CENTER;
    box->align_y() = ui::Box::Align::CENTER;

    box->append(text2.release());
    box->append(text1.release());
    box->convert_coords(_ui->res());
    box->origin() = ui::Point {0, 0};
    box->dimension() = _ui->res();
    _ui->add_layer("endingmenu", box.release());

    auto *ret = _ui->get(0, "1.2");
    auto *congrats = _ui->get(0, "1.1");
    ret->on_click([this](void*) mutable {
        _active = false;
    });
    auto renderer = [this](ui::Renderable *t) mutable {render_text(t, glm::vec3(1, 1, 1));};
    ret->render_with(renderer);
    congrats->render_with(renderer);
    auto d = [](void *t){delete (text_t*)t;};
    ret->payload(d);
    congrats->payload(d);
    ret->payload() = nullptr;
    congrats->payload() = nullptr;
}
