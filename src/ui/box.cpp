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
#include "box.hpp"

using namespace ui;

void Box::render() {
    Renderable::render();
    for(size_t i = 0; i < count(); ++i) {
        operator[](i)->render();
    }
}

void Box::set_root(Renderable *r) {
    Renderable::set_root(r);
    for(size_t i = 0; i < count(); ++i) {
        operator[](i)->set_root(r);
    }
}

Renderable *Box::get(const char* coord) {
    char* next;
    int i = strtol(coord, &next, 10);
    assert(i > 0);
    assert(*next == '\0' || *next == '.');
    if(_orientation == VERTICAL) i = count() - i + 1;
    if(*next == '\0') return operator[](i - 1);
    return operator[](i - 1)->get(next + 1);
}

void Box::show() {
    Renderable::show();
    for(size_t i = 0; i < count(); ++i) {
        operator[](i)->show();
    }
}

void Box::hide() {
    Renderable::show();
    for(size_t i = 0; i < count(); ++i) {
        operator[](i)->hide();
    }
}

bool Box::click(Point p) {
    for(size_t i = 0; i < count(); ++i) {
        if(operator[](i)->click(p)) return true;
    }
    return false;
}

void Box::convert_coords(Point mul) {
    Area::convert_coords(mul);
    for(size_t i = 0; i < count(); ++i) {
        operator[](i)->convert_coords(mul);
    }
}

void Box::__introspect(size_t off) {
    Renderable::__introspect(off);
    for(unsigned int i = 0; i < count(); ++i) {
        operator[](i)->__introspect(off + 2);
    }
}

void Box::recalculate_origin() {
    Point current_origin = origin();
    Renderable *filling = nullptr;
    Point center_offset = Point {0, 0};
    // set all except filling
    for(unsigned int i = 0; i < count(); ++i) {
        auto *it = operator[](i);
        if(it->expand()) {
            assert(filling == nullptr);
            filling = it;
            it->origin() = current_origin;
        } else {
            it->origin() = current_origin;
            if(_orientation == VERTICAL) current_origin.y += it->dimension().y;
            else current_origin.x += it->dimension().x;
            center_offset += it->dimension();
        }
        it->recalculate_origin();
    }

    if(_orientation == VERTICAL) {
        int xcenter;
        switch(_x) {
            case BEGINNING: break; // nothing to do here
            case CENTER:
                xcenter = (origin() + dimension() / Point {2, 2}).x;
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x = xcenter - it->dimension().x / 2;
                }
                break;
            case END:
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x = dimension().x - it->origin().x - it->dimension().x;
                }
                break;
        }
        switch(_y) {
            case BEGINNING: break; // nothing to do here
            case CENTER:
                if(filling != nullptr) break; // nothing to do if entire length is occupied
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y += center_offset.y;
                }
                break;
            case END:
                current_origin = origin() + dimension(); // TODO fix other END orientations
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y = current_origin.y - it->dimension().y;
                    current_origin.y -= it->dimension().y;
                }
                break;
        }
    } else { // == HORIZONTAL
        int ycenter;
        switch(_y) {
            case BEGINNING: break; // nothing to do here
            case CENTER:
                ycenter = (origin() + dimension() / Point {2, 2}).y;
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y = ycenter - it->dimension().y / 2;
                }
                break;
            case END:
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().y = dimension().y - it->origin().y - it->dimension().y;
                }
                break;
        }
        switch(_x) {
            case BEGINNING: break; // nothing to do here
            case CENTER:
                if(filling != nullptr) break; // nothing to do if entire length is occupied
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x += center_offset.x;
                }
                break;
            case END:
                current_origin = origin() + dimension();
                for(unsigned int i = 0; i < count(); ++i) {
                    auto *it = operator[](i);
                    it->origin().x = current_origin.x - it->dimension().x;
                    current_origin.x -= it->dimension().x;
                }
                break;
        }
    }
}

void Box::recalculate_dimension() {
    Point dold = dimension() - padding();
    dimension() = Point {0, 0};
    Renderable *filling = nullptr;
    for(unsigned int i = 0; i < count(); ++i) {
        auto *it = operator[](i);
        if(it->expand()) {
            assert(filling == nullptr);
            filling = it;
        } else {
            it->recalculate_dimension();
            if(_orientation == VERTICAL) {
                auto d = it->dimension();
                dimension().x = std::max(dimension().x, d.x);
                dimension().y += d.y;
            } else {
                auto d = it->dimension();
                dimension().y = std::max(dimension().y, d.y);
                dimension().x += d.x;
            }
        }
    }
    if(filling != nullptr) {
        if(_orientation == VERTICAL) {
            filling->dimension().x = std::max(dold.x, dimension().x);
            assert(dold.y >= dimension().y);
            filling->dimension().y = dold.y - dimension().y;
            dimension().y = dold.y + padding().y;
        } else {
            filling->dimension().y = std::max(dold.y, dimension().y);
            assert(dold.x >= dimension().x);
            filling->dimension().x = dold.x - dimension().x;
            dimension().x = dold.x + padding().x;
        }
        filling->recalculate_dimension();
    } else {
        dimension().x = std::max(dimension().x, dold.x) + padding().x;
        dimension().y = std::max(dimension().y, dold.y) + padding().y;
    }
}
