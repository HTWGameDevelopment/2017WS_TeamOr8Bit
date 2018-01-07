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

#include<iostream>
#include<functional>
#include<memory>
#include<vector>

namespace ui {

    typedef float AbstractNumber;
    typedef int DefinedNumber;

    template<typename T>
    struct Point {
    public:
        T x;
        T y;
        bool operator==(const Point<T> &a) {
            return x == a.x && y == a.y;
        }
        bool operator!=(const Point<T> &a) {
            return operator==(a) == false;
        }
        bool operator<(const Point<T> &a) {
            return x < a.x && y < a.y;
        }
        bool operator>=(const Point<T> &a) {
            return x >= a.x && y >= a.y;
        }
        bool operator>(const Point<T> &a) {
            return x > a.x && y > a.y;
        }
        bool operator<=(const Point<T> &a) {
            return x <= a.x && y <= a.y;
        }
        Point<T> &operator+=(const Point<T> &a) {
            x += a.x;
            y += a.y;
            return *this;
        }
    };

    typedef Point<AbstractNumber> absp_t;
    typedef Point<DefinedNumber> defp_t;

    inline defp_t operator*(defp_t a, absp_t d) {
        return defp_t {(int)(a.x * d.x), (int)(a.y * d.y)};
    }

    template<typename T> Point<T> operator+(Point<T> a, Point<T> b) {return Point<T> {a.x + b.x, a.y + b.y};}
    template<typename T> Point<T> operator-(Point<T> a, Point<T> b) {return Point<T> {a.x - b.x, a.y - b.y};}
    template<typename T> Point<T> operator/(Point<T> a, Point<T> b) {return Point<T> {a.x / b.x, a.y / b.y};}

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

    class DefinedArea {
    private:
        defp_t _origin;
        defp_t _size;
    public:
        defp_t &origin() {
            return _origin;
        }
        defp_t &dimension() {
            return _size;
        }
        virtual void recalculate() {}
        virtual bool is_dynamic() {return false;}
    };

    class DefinedRenderable: public DefinedArea {
    private:
        std::function<void(void)> _render;
        std::function<void(void)> _click;
    public:
        template<typename F> void render_with(F &&r) {
            _render = std::move(r);
        }
        template<typename F> void on_click(F &&c) {
            _click = std::move(c);
        }
        virtual DefinedRenderable *get(const char* str) {
            return this;
        }
        virtual void render() {
            _render();
        }
        virtual void debug(unsigned int off) {
            std::cout << std::string(" ", off)
                << "[" << origin().x
                << "," << origin().y
                << "] dim ["
                << dimension().x
                << ","
                << dimension().y
                << "] "
                << (_click ? "C" : "")
                << (_render ? "R" : "")
                << std::endl;
        }
        virtual bool click(defp_t p) {
            if(!_click) return false;
            defp_t t = p - origin();
            if(t >= defp_t {0, 0} && t <= dimension()) {
                _click();
                return true;
            }
            return false;
        }
    };

    class AbstractRenderable {
    public:
        virtual DefinedRenderable *buildDefined(defp_t res) = 0;
    };

    template<typename T>
    class Container {
    private:
        std::vector<std::unique_ptr<T>> _items;
    public:
        void append(T *item) {_items.emplace_back(item);}
        T *operator[](unsigned int i) {return _items[i].get();}
        unsigned int count() {return _items.size();}
    };

    typedef Container<AbstractRenderable> AbstractContainer;
    typedef Container<DefinedRenderable> DefinedContainer;

}

#endif
