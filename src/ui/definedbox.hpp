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
#ifndef UI_DEFINED_BOX_HPP
#define UI_DEFINED_BOX_HPP

#include<ui/abstract_common.hpp>
#include<ui/abstractbox.hpp>

#include<assert.h>

namespace ui {

    class DefinedBox: public DefinedRenderable, public DefinedContainer {
    private:
        AbstractBox::Orientation _orientation;
        AbstractBox::Growth _growth;
        AbstractBox::Align _x;
        AbstractBox::Align _y;
    public:
        DefinedBox(AbstractBox::Orientation o, AbstractBox::Growth g, AbstractBox::Align x, AbstractBox::Align y);
        void render();
        virtual void recalculate();
        virtual bool is_dynamic();
        virtual DefinedRenderable *get(const char* coord) {
            char* next;
            int i = strtol(coord, &next, 10);
            assert(i > 0);
            assert(*next == '\0' || *next == '.');
            if(*next == '\0') return operator[](i - 1);
            return operator[](i - 1)->get(next + 1);
        }
        virtual void debug(unsigned int off) {
            DefinedRenderable::debug(off);
            for(unsigned int i = 0; i < count(); ++i)
                operator[](i)->debug(off + 2);
        }
        virtual bool click(defp_t p) {
            for(unsigned int i = 0; i < count(); ++i) {
                if(operator[](i)->click(p)) return true;
            }
            return false;
        }
    };

}

#endif
