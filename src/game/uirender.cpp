#include "uirender.hpp"

#include<ui/text.hpp>

using namespace gamespace;

void gamespace::render_text(ui::Renderable *t, glm::vec3 color) {
    if(t->payload() == nullptr) {
        t->payload() = new text_t(
            ((ui::Text*)t)->text(),
            qe::Cache::glyphlatin,
            to_ivec2(t->origin() + t->margin() + t->padding() + ui::Point {0, 0.5} * (t->dimension() - t->margin() - t->padding())),
            (int)(0.5 * (t->dimension().y - t->margin().y - t->padding().y)),
            (int)(t->dimension().x - t->margin().x - t->padding().x));
    }
    text_t *pl = (text_t*)t->payload();
    pl->foreground() = color;
    pl->render();
}

void gamespace::render_rectangle(ui::Point a, ui::Point b, glm::vec3 bg, glm::vec2 res) {
    glm::vec2 origin = glm::vec2(a.x, a.y) * glm::vec2(2, 2) / res;
    glm::vec2 size = glm::vec2(b.x, b.y) * glm::vec2(2, 2) / res;

    origin -= glm::vec2(1, 1);

    qe::Cache::sprite2dcolor->use();
    // set shader
    qe::Cache::sprite2dcolor->setUniform<qe::UNIORIGIN>(origin);
    qe::Cache::sprite2dcolor->setUniform<qe::UNISIZE>(size);
    qe::Cache::sprite2dcolor->setUniform<qe::UNITEXTBG>(bg);
    // set vao
    qe::Cache::meshm->render();
}
