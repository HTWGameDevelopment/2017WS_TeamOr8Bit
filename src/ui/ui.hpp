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
#ifndef UI_UI_HPP
#define UI_UI_HPP

#include<ui/common.hpp>
#include<ui/box.hpp>

namespace ui {

    class UI {
    private:
        std::unique_ptr<Renderable> _container;
        std::vector<std::unique_ptr<Renderable>> _context_menus;
        Point _res;
    public:
        UI(Point res): _res(res) {}
        Point res() {
            return _res;
        }
        void set_container(Renderable *r) {
            _container.reset(r);
            _container->set_root(r);
            _container->dimension() = _res;
            _container->origin() = Point {0, 0};
            _container->recalculate_dimension();
            _container->recalculate_origin();
        }
        void add_context_menu(Renderable *r) {
            _context_menus.emplace_back(r);
            r->set_root(r);
            r->recalculate_dimension();
            r->recalculate_origin();
            r->show();
        }
        void hide_context_menu(Renderable *ui) {
            assert(ui);
            for(auto i = _context_menus.begin(); i != _context_menus.end(); ++i) {
                if(i->get() == ui) {
                    ui->hide();
                    _context_menus.erase(i);
                    return;
                }
            }
        }
        void render() {
            if(_container.get()) _container->render();
            for(auto &i : _context_menus) {
                if(i->enabled()) i->render();
            }
        }
        Renderable *get(const char* coord) {
            char* next;
            int i = strtol(coord, &next, 10);
            assert(i == 1);
            assert(*next == '\0' || *next == '.');
            if(*next == '\0') return _container.get();
            return _container->get(next + 1);
        }
        void __introspect(size_t off = 0) {
            std::cout << std::string(off, ' ') << "UI[" << _res.x << "," << _res.y << "]" << std::endl;
            if(_container.get()) _container->__introspect(off + 2);
            for(auto &i : _context_menus)
                i->__introspect(off + 2);
        }
        bool click(Point pos) {
            if(_container.get() && _container->click(pos)) return true;
            for(auto &i : _context_menus)
                if(i->click(pos)) return true;
            return false;
        }
        Renderable *hovers(ui::Point pos) {
            for(auto &i : _context_menus) {
                if(i->hovers(pos)) return i.get();
            }
            return nullptr;
        }
    };

}

#endif
