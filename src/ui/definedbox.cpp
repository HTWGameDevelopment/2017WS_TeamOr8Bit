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
            } else if(past_filling) {
                if(_orientation == AbstractBox::VERTICAL) it->origin().y += filling->dimension().y;
                else it->origin().x += filling->dimension().x;
            }
        }
    }

    switch(_x) {
        case AbstractBox::BEGINNING: break; // nothing to do here
        case AbstractBox::CENTER:

            break;
        case AbstractBox::END:

            break;
    }

    return;
}
