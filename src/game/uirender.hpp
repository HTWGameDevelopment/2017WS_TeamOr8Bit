#ifndef GAMESPACE_UIRENDER_HPP
#define GAMESPACE_UIRENDER_HPP

#include<engine/cache.hpp>
#include<engine/text.hpp>

#include<game/gameboard.hpp>

#include<ui/common.hpp>

namespace gamespace {

    typedef decltype(qe::Cache::glyphlatin) glyphmap;
    typedef qe::Text<std::remove_pointer<glyphmap>::type> text_t;

    template<typename T>
    glm::ivec2 to_ivec2(T t) {
        return glm::ivec2(t.x, t.y);
    }

    void render_text(ui::Renderable *t, glm::vec3 color);
    void render_rectangle(ui::Point a, ui::Point b, glm::vec3 bg, glm::vec2 res);

}

#endif
