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
#ifndef UI_COMMON_HPP
#define UI_COMMON_HPP

#include<assert.h>
#include<iostream>
#include<functional>
#include<memory>
#include<vector>

namespace ui {

    struct Point {
    public:
        float x;
        float y;
        bool operator==(const Point &a) {
            return x == a.x && y == a.y;
        }
        bool operator!=(const Point &a) {
            return operator==(a) == false;
        }
        bool operator<(const Point &a) {
            return x < a.x && y < a.y;
        }
        bool operator>=(const Point &a) {
            return x >= a.x && y >= a.y;
        }
        bool operator>(const Point &a) {
            return x > a.x && y > a.y;
        }
        bool operator<=(const Point &a) {
            return x <= a.x && y <= a.y;
        }
        Point &operator+=(const Point &a) {
            x += a.x;
            y += a.y;
            return *this;
        }
        Point &operator-=(const Point &a) {
            x -= a.x;
            y -= a.y;
            return *this;
        }
        Point &operator*=(const Point &a) {
            x *= a.x;
            y *= a.y;
            return *this;
        }
    };

    inline Point operator+(Point a, Point b) {return Point {a.x + b.x, a.y + b.y};}
    inline Point operator-(Point a, Point b) {return Point {a.x - b.x, a.y - b.y};}
    inline Point operator*(Point a, Point b) {return Point {a.x * b.x, a.y * b.y};}
    inline Point operator/(Point a, Point b) {return Point {a.x / b.x, a.y / b.y};}

    class Area {
    private:
        Point _origin;
        Point _dimension;
        Point _margin;
        Point _padding;
        bool _expand;
    public:
        Point &origin() {
            return _origin;
        }
        Point &dimension() {
            return _dimension;
        }
        Point &margin() {
            return _margin;
        }
        Point &padding() {
            return _padding;
        }
        bool &expand() {
            return _expand;
        }
        virtual void recalculate_origin() {}
        virtual void recalculate_dimension() {}
        virtual void convert_coords(Point mul) {
            _origin *= mul;
            _dimension *= mul;
            _margin *= mul;
            _padding *= mul;
        }
    };

    class Renderable: public Area {
    private:
        std::function<void(Renderable*)> _render;
        std::function<void(Renderable*)> _click;
        std::function<void(void*)> _deleter;
        void *_payload;
        Renderable *_root;
        bool _enabled;
    public:
        virtual ~Renderable() {
            if(_deleter) _deleter(_payload);
        }
        template<typename F> void render_with(F &&r) {
            _render = std::move(r);
        }
        template<typename F> void on_click(F &&c) {
            _click = std::move(c);
        }
        void *&payload() {
            return _payload;
        }
        void delete_payload() {
            if(_payload == nullptr) return;
            assert(_deleter);
            _deleter(_payload);
            _payload = nullptr;
        }
        template<typename F> void payload(F &&d) {
            _deleter = d;
        }
        virtual Renderable *get(const char* str) {
            return this;
        }
        virtual void set_root(Renderable *r) {
            _root = r;
        }
        virtual void render() {
            if(_root != this) origin() += _root->origin();
            if(_render) _render(this);
            if(_root != this) origin() -= _root->origin();
        }
        virtual void __introspect(size_t off) {
            std::cout << std::string(off, ' ')
                << "[" << origin().x
                << "," << origin().y
                << "] dim ["
                << dimension().x
                << ","
                << dimension().y
                << "] pad ["
                << padding().x
                << ","
                << padding().y
                << "] mgn ["
                << margin().x
                << ","
                << margin().y
                << "] "
                << (_click ? "C" : "")
                << (_render ? "R" : "")
                << (_enabled ? "V" : "H")
                << std::endl;
        }
        bool hovers(Point p) {
            Point t = p - origin() - padding();
            if(_root != this) t -= _root->origin();
            if(t >= Point {0, 0} && t <= dimension() - padding()) {
                return true;
            }
            return false;
        }
        virtual bool click(Point p) {
            if(!_click) return false;
            if(hovers(p)) {
                _click(this);
                return true;
            }
            return false;
        }
        virtual void show() {_enabled = true;}
        virtual void hide() {_enabled = false;}
        bool enabled() {return _enabled;}
    };

    class Container {
    private:
        std::vector<std::unique_ptr<Renderable>> _items;
    public:
        virtual ~Container() {}
        void append(Renderable *item) {_items.emplace_back(item);}
        Renderable *operator[](unsigned int i) {return _items[i].get();}
        unsigned int count() {return _items.size();}
    };

}

#endif
