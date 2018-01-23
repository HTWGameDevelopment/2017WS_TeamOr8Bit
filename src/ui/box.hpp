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
#ifndef UI_BOX_HPP
#define UI_BOX_HPP

#include<ui/common.hpp>

namespace ui {

    class Box: public Renderable, public Container {
    public:
        enum Orientation { HORIZONTAL, VERTICAL };
        enum Align { BEGINNING, CENTER, END };
    private:
        Orientation _orientation;
        Align _x;
        Align _y;
    public:
        Orientation &orientation() {
            return _orientation;
        }
        Align &align_x() {
            return _x;
        }
        Align &align_y() {
            return _y;
        }
        virtual Box &operator=(Box &&other) {
            if(this == &other) return *this;
            Renderable::operator=(std::move(other));
            Container::operator=(std::move(other));
            _orientation = other._orientation;
            _x = other._x;
            _y = other._y;
            return *this;
        }
        virtual void render();
        virtual void set_root(Renderable *r);
        virtual Renderable *get(const char* str);
        virtual void show();
        virtual void hide();
        virtual bool click(Point p);
        virtual void recalculate_origin();
        virtual void recalculate_dimension();
        virtual void convert_coords(Point mul);
        virtual void __introspect(size_t off);
    };

}

#endif
