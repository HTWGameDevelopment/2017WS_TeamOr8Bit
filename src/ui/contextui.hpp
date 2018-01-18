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
#ifndef UI_CONTEXT_UI_HPP
#define UI_CONTEXT_UI_HPP

#include<ui/abstract_common.hpp>

#include<iostream>
#include<assert.h>

namespace ui {

    class DefinedContextUI;
    class DefinedUI;

    class ContextUIModel {
    public:
        virtual void init(DefinedContextUI *ui) = 0;
        virtual void on_show() = 0;
        virtual void on_hide() = 0;
    };

    class DefinedContextUI: public DefinedRenderable {
    private:
        std::unique_ptr<DefinedRenderable> _inner;
        ContextUIModel *_model;
        DefinedUI *_ui;
    public:
        DefinedContextUI() {}
        void set_inner(DefinedRenderable *r) {
            _inner.reset(r);
        }
        void set_model(ContextUIModel *model) {
            assert(model);
            _model = model;
            model->init(this);
        }
        void set_ui(DefinedUI *ui) {
            _ui = ui;
        }
        DefinedUI *ui() {
            return _ui;
        }
        virtual void recalculate_dimension() {
            _inner->dimension() = dimension();
            _inner->recalculate_dimension();
            dimension() = _inner->dimension();
        }
        virtual void recalculate_origin() {
            origin() = defp_t {0, 0};
            _inner->origin() = origin();
            _inner->recalculate_origin();
        }
        virtual void render() {
            _inner->render(origin());
        }
        virtual DefinedRenderable *get(const char* str) {
            return _inner->get(str);
        }
        virtual bool click(defp_t p) {
            assert(_inner.get());
            return _inner->click(p);
        }
        virtual void __introspect(size_t off) {
            DefinedRenderable::__introspect(off);
            assert(_inner.get());
            _inner->__introspect(off + 2);
        }
    };

    class AbstractContextUI: public AbstractRenderable, public AbstractArea {
    private:
        std::unique_ptr<AbstractRenderable> _inner;
    public:
        void set_inner(AbstractRenderable *r) {
            _inner.reset(r);
        }
        DefinedContextUI *buildDefined(defp_t res) {
            std::unique_ptr<DefinedContextUI> cui(new DefinedContextUI());
            auto *inner = _inner->buildDefined(res);
            cui->set_inner(inner);
            cui->dimension() = inner->dimension();
            cui->origin() = inner->origin();
            return cui.release();
        }
    };

}

#endif
