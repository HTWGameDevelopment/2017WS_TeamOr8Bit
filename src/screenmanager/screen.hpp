// Copyright (c) 2017 Fabian Stiewitz
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
#ifndef SCREEN_HPP
#define SCREEN_HPP

#include<memory>
#include<type_traits>
#include<vector>

namespace screen {
    template<typename T>
    class ScreenManager {
    private:
        std::vector<std::unique_ptr<T>> _screens;
        T *_active_screen = nullptr;
        T *_next_screen;
        bool _stopped;
    public:
        template<typename I>
        I *addScreen(I *obj) {
            assert(obj);
            _screens.emplace_back(obj);
            return obj;
        }
        void changeActiveScreen(T &target) {
            if(_active_screen) _active_screen->deactivate_screen();
            _next_screen = &target;
            _stopped = false;
        }
        void nextScreen() {
            if(_next_screen) {
                _active_screen = _next_screen;
                _next_screen = nullptr;
                _active_screen->activate_screen();
            } else _stopped = true;
        }
        bool stopped() {
            return _stopped;
        }
        void quit() {
            if(_active_screen) _active_screen->deactivate_screen();
            _next_screen = nullptr;
        }
        T &getScreen(unsigned int k) {
            return *_screens[k].get();
        }
        void input_key(int key, int action) {
            assert(_active_screen);
            _active_screen->key_callback(key, action);
        }
        void input_mouse(double x, double y) {
            assert(_active_screen);
            _active_screen->mouse_callback(x, y);
        }
        void input_button(int button, int action, int mods) {
            assert(_active_screen);
            _active_screen->mouse_button_callback(button, action, mods);
        }
        void input_idle() {
            assert(_active_screen);
            _active_screen->idle_callback();
        }
    };
}

#endif
