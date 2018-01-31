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

    struct layer_t {
        std::string name;
        std::unique_ptr<Renderable> layer;
    };

    class UI {
    private:
        std::vector<layer_t> _layers;
        Point _res;
    public:
        UI(Point res): _res(res) {}
        Point res() {
            return _res;
        }
        void add_layer(std::string name, Renderable *r) {
            _layers.push_back(layer_t {name, std::unique_ptr<Renderable>(r)});
            r->set_root(r);
            r->recalculate_dimension();
            r->recalculate_origin();
            r->show();
        }
        void hide_context_menu(Renderable *ui) {
            assert(ui);
            for(auto i = _layers.begin(); i != _layers.end(); ++i) {
                if(i->layer.get() == ui) {
                    ui->hide();
                    _layers.erase(i);
                    return;
                }
            }
        }
        template<typename F>
        bool hasModelMatching(F &&f) {
            for(auto &c : _layers) {
                if(f(c.name, c.layer->payload())) return true;
            }
            return false;
        }
        void render() {
            for(auto &i : _layers) {
                if(i.layer->enabled()) i.layer->render();
            }
        }
        Renderable *get(unsigned int layer, const char* coord) {
            char* next;
            int i = strtol(coord, &next, 10);
            assert(i == 1);
            assert(*next == '\0' || *next == '.');
            if(*next == '\0') return _layers[layer].layer.get();
            return _layers[layer].layer->get(next + 1);
        }
        void __introspect(size_t off = 0) {
            std::cout << std::string(off, ' ') << "UI[" << _res.x << "," << _res.y << "]" << std::endl;
            for(auto &i : _layers)
                i.layer->__introspect(off + 2);
        }
        bool click(Point pos) {
            for(auto &i : _layers)
                if(i.layer->click(pos)) return true;
            return false;
        }
        Renderable *hovers(ui::Point pos) {
            for(auto &i : _layers) {
                if(i.layer->hovers(pos)) return i.layer.get();
            }
            return nullptr;
        }
    };

}

#endif
