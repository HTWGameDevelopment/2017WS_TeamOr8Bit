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
#ifndef UI_ABSTRACT_BOX_HPP
#define UI_ABSTRACT_BOX_HPP

#include<ui/abstract_common.hpp>

namespace ui {

    class AbstractBox: public AbstractRenderable, public AbstractArea, public AbstractContainer {
    public:
        enum Orientation { HORIZONTAL, VERTICAL };
        enum Growth { MINIMUM, FILL };
        enum Align { BEGINNING, CENTER, END };
    private:
        Orientation _orientation;
        Growth _growth;
        Align _x;
        Align _y;
    public:
        void set_orientation(Orientation o) {_orientation = o;}
        void set_growth(Growth g) {_growth = g;}
        void set_align_inner(Align x, Align y) {_x = x; _y = y;}
        DefinedRenderable *buildDefined(defp_t res);
    };

}

#endif