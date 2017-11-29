// Copyright (c) 2017 Fabian Stiewitz
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
    public:
        Text() {}
        Text(glyphmap *g, glm::ivec2 baseline, PositionMode mode = BOTTOM): _glyphmap(g), _baseline(baseline) {
            switch(mode) {
            case TOP:
                _baseline.y = _glyphmap->getResolution().y - _glyphmap->highestGlyph() - _baseline.y;
                break;
            }
        }
        void render() {
            qe::Cache::texts->use();
            glm::ivec2 pos = _baseline;

            for(size_t i = 0; i < _text.length(); ++i) { // TODO Unicode support?
                fontmetrics metrics = _glyphmap->getMetrics(_text[i]);
                qe::Cache::texts->setUniform<UNIM>(_glyphmap->scalePosition(pos, metrics));
                qe::Cache::texts->setUniform<UNITEXTUVP>(_glyphmap->scaleOrigin(metrics));
                qe::Cache::texts->setUniform<UNITEXTUVS>(_glyphmap->scaleUVScale(metrics));
                qe::Cache::meshm->render();
                pos.x += metrics.adv_x;
            }
        }
        std::string &text() {
            return _text;
        }
    };

}

#endif
