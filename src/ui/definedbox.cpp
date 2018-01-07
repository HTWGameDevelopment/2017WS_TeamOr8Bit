// Copyright (c) 2018 Fabian Stiewitz
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
#include "definedbox.hpp"

#include<assert.h>

using namespace ui;

DefinedBox::DefinedBox(AbstractBox::Orientation o, AbstractBox::Growth g, AbstractBox::Align x, AbstractBox::Align y)
: _orientation(o), _growth(g), _x(x), _y(y) {

}

void DefinedBox::render() {
    for(unsigned int i = 0; i < count(); ++i) {
        operator[](i)->render();
    }
}

bool DefinedBox::is_dynamic() {
    return _growth == AbstractBox::FILL;
}

void DefinedBox::recalculate() {
    defp_t current_origin = origin();
    DefinedRenderable *filling = nullptr;
    defp_t center_offset = defp_t {0, 0};
    // set all except filling
    for(unsigned int i = 0; i < count(); ++i) {
        auto *it = operator[](i);
        if(it->is_dynamic()) {
            assert(filling == nullptr);
            filling = it;
            it->origin() = current_origin;
        } else {
            it->origin() = current_origin;
            if(_orientation == AbstractBox::VERTICAL) current_origin.y += it->dimension().y;
            else current_origin.x += it->dimension().x;
            center_offset += it->dimension();
        }
    }

    if(filling != nullptr) {
        // offset by filling
        bool past_filling = false;
        for(unsigned int i = 0; i < count(); ++i) {
            auto *it = operator[](i);
            if(it->is_dynamic()) {
                past_filling = true;
                if(_orientation == AbstractBox::VERTICAL) it->dimension() = dimension() - defp_t {0, current_origin.y};
                else it->dimension() = dimension() - defp_t {current_origin.x, 0};
                center_offset += it->dimension();
            } else if(past_filling) {
                if(_orientation == AbstractBox::VERTICAL) it->origin().y += filling->dimension().y;
                else it->origin().x += filling->dimension().x;
            }
        }
    }

    center_offset = (dimension() - center_offset ) / defp_t {2, 2};

    if(_orientation == AbstractBox::VERTICAL) {
        int xcenter;
        switch(_x) {
            case AbstractBox::BEGINNING: break; // nothing to do here
            case AbstractBox::CENTER:
                xcenter = (origin() + dimension() / defp_t {2, 2}).x;
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x = xcenter - it->dimension().x / 2;
                }
                break;
            case AbstractBox::END:
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x = dimension().x - it->origin().x - it->dimension().x;
                }
                break;
        }
        switch(_y) {
            case AbstractBox::BEGINNING: break; // nothing to do here
            case AbstractBox::CENTER:
                if(filling != nullptr) break; // nothing to do if entire length is occupied
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y += center_offset.y;
                }
                break;
            case AbstractBox::END:
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y = dimension().y - it->origin().y - it->dimension().y;
                }
                break;
        }
    } else { // == HORIZONTAL
        int ycenter;
        switch(_y) {
            case AbstractBox::BEGINNING: break; // nothing to do here
            case AbstractBox::CENTER:
                ycenter = (origin() + dimension() / defp_t {2, 2}).y;
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y = ycenter - it->dimension().y / 2;
                }
                break;
            case AbstractBox::END:
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y = dimension().y - it->origin().y - it->dimension().y;
                }
                break;
        }
        switch(_x) {
            case AbstractBox::BEGINNING: break; // nothing to do here
            case AbstractBox::CENTER:
                if(filling != nullptr) break; // nothing to do if entire length is occupied
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x += center_offset.x;
                }
                break;
            case AbstractBox::END:
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x = dimension().x - it->origin().x - it->dimension().x;
                }
                break;
        }
    }

    return;
}
