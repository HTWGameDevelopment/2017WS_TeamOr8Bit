// Copyright (c) 2017-2018 Fabian Stiewitz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef QE_TEXT_HPP
#define QE_TEXT_HPP

#include<engine/glyphmap.hpp>
#include<engine/cache.hpp>

namespace qe {

    /**
     * \brief Anchoring of text
     */
    enum PositionMode {BOTTOM, TOP};

    /**
     * \brief Represents text on screen
     */
    template<typename G>
    class Text {
    public:
        typedef G glyphmap; //!< Glyphmap type
    private:
        std::string _text; //!< text
        glyphmap *_glyphmap; //!< Glyphmap
        glm::ivec2 _baseline; //!< Starting position of baseline
        float _scale;
        glm::vec3 _foreground; //!< text color
    public:
        Text(): _foreground(glm::vec3(1, 1, 1)) {}
        Text(glyphmap *g, glm::ivec2 baseline, PositionMode mode = BOTTOM): _glyphmap(g), _baseline(baseline), _scale(1), _foreground(glm::vec3(1, 1, 1)) {
            switch(mode) {
            case TOP:
                _baseline.y = _glyphmap->getResolution().y - _glyphmap->highestGlyph() - _baseline.y;
                break;

            case BOTTOM:
                break; // ignore
            }
        }
        Text(std::string text, glyphmap *g, glm::ivec2 baseline, int maxheight, int maxlength): _text(text), _glyphmap(g), _foreground(glm::vec3(1, 1, 1)) {
            scale_text(baseline, maxheight, maxlength);
        }
        glm::vec3 &foreground() {
            return _foreground;
        }
        void render() {
            qe::Cache::texts->use();
            glm::ivec2 pos = _baseline;

            for(size_t i = 0; i < _text.length(); ++i) { // TODO Unicode support?
                if(_text[i] == '\n') {
                    pos.x = _baseline.x;
                    pos.y += _glyphmap->linespace();
                } else {
                    fontmetrics metrics = _glyphmap->getMetrics(_text[i]);
                    qe::Cache::texts->setUniform<UNIM>(_glyphmap->scalePosition(pos, metrics, _scale));
                    qe::Cache::texts->setUniform<UNITEXTUVP>(_glyphmap->scaleOrigin(metrics));
                    qe::Cache::texts->setUniform<UNITEXTUVS>(_glyphmap->scaleUVScale(metrics));
                    qe::Cache::texts->setUniform<UNITEXTFG>(_foreground);
                    qe::Cache::meshm->render();
                    pos.x += metrics.adv_x * _scale;
                }
            }
        }
        void scale_text(glm::ivec2 baseline, int maxheight, int maxlength) {
            _baseline = baseline;
            ssize_t px = 0;
            ssize_t yb = 0;
            for(size_t i = 0; i < _text.length(); ++i) { // TODO Unicode support?
                if(_text[i] == '\n') {
                    yb += _glyphmap->linespace();
                } else {
                    fontmetrics metrics = _glyphmap->getMetrics(_text[i]);
                    px += metrics.adv_x;
                    yb = std::max(yb, metrics.off_y);
                }
            }

            glm::vec2 scale = glm::vec2(1.0 * maxlength / px, 1.0 * maxheight / yb);
            //_scale = std::min(scale.x, scale.y);
            _scale = std::min(scale.y, 1.0f);
        }
        std::string &text() {
            return _text;
        }
    };

}

#endif
