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
#ifndef UI_DEFINED_UI_HPP
#define UI_DEFINED_UI_HPP

#include<ui/abstract_common.hpp>
#include<ui/contextui.hpp>

#include<iostream>

#include<assert.h>
#include<stdlib.h>

namespace ui {

    class DefinedUI {
    private:
        std::unique_ptr<DefinedRenderable> _container;
        std::vector<std::unique_ptr<DefinedContextUI>> _context_menus;
        DefinedContextUI *_active_context_menu;
        defp_t _res;
    public:
        DefinedUI(DefinedNumber resx, DefinedNumber resy): _active_context_menu{nullptr}, _res{resx, resy} {}
        void set_container(DefinedRenderable *r) {
            _container.reset(r);
            _container->set_root(_container.get());
        }
        void add_context_menu(DefinedContextUI *r) {
            _context_menus.emplace_back(r);
            r->recalculate_dimension();
            r->recalculate_origin();
        }
        defp_t resolution() {return _res;}
        void recalculate();
        void render() {
            if(_container.get()) _container->render();
            if(_active_context_menu) _active_context_menu->render();
        }
        DefinedRenderable *get(const char* coord) {
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
        void show_context_menu(DefinedContextUI *ui) {
            if(_active_context_menu == ui) return;
            if(_active_context_menu && _active_context_menu != ui) _active_context_menu->hide();
            _active_context_menu = ui;
            _active_context_menu->on_show();
        }
        void hide_context_menu(DefinedContextUI *ui) {
            assert(ui);
            if(ui != _active_context_menu) return;
            _active_context_menu->on_hide();
            _active_context_menu = nullptr;
        }
        void click(defp_t pos) {
            if(_active_context_menu && _active_context_menu->click(pos)) return;
            if(_container.get()) _container->click(pos);
        }
    };

}

#endif
