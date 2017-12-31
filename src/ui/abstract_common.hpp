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
#ifndef UI_ABSTRACT_COMMON_HPP
#define UI_ABSTRACT_COMMON_HPP

#include<memory>
#include<vector>

namespace ui {

    typedef float AbstractNumber;

    template<typename T>
    class Point {
    private:
        T _x;
        T _y;
    public:
        T &x() {return _x;}
        T &y() {return _y;}
    };

    typedef Point<AbstractNumber> absp_t;

    class AbstractArea {
    private:
        absp_t _origin;
        absp_t _size;
    public:
        absp_t &origin() {
            return _origin;
        }
        absp_t &dimension() {
            return _size;
        }
    };

    class Renderable {
    public:
        virtual void render() {}
    };

    class Container {
    private:
        std::vector<std::unique_ptr<Renderable>> _items;
    public:
        template<typename T> void append(T &&item) {_items.emplace_back(new T(std::forward<T>(item)));}
        Renderable *operator[](unsigned int i) {return _items[i].get();}
        unsigned int count() {return _items.size();}
    };

}

#endif
