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
#ifndef QE_CONTEXT_HPP
#define QE_CONTEXT_HPP

#include<engine/constants.hpp>

#include<logger.h>

#include<glm/vec2.hpp>

#include<cassert>
#include<chrono>
#include<stdexcept>
#include<string>

namespace qe {

struct backend_error: public std::runtime_error {
    backend_error(std::string s): runtime_error(s) {}
};

void keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void errorcallback(int code, const char* desc);
void mousecallback(GLFWwindow*, double, double);
void idlecallback();

class Context {
private:
    unsigned int _w, _h;
    std::string _title;
    GLFWwindow *_window;
    enum _context_t {NONE,MESH} context = NONE;
    double _lasttime;
    double _deltat;
    unsigned int _fps;
    double _lastfpstime;
    unsigned int _fpsc;
public:
    Context(std::string title, unsigned int w, unsigned int h): _w(w), _h(h), _title(title) {
        assert(_w != 0);
        assert(_h != 0);
        if(!glfwInit())
            throw backend_error(backend_error_glfw);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        _window = glfwCreateWindow(_w, _h, _title.c_str(), NULL, NULL);

        if(!_window) {
            glfwTerminate();
            throw backend_error(backend_error_window);
        }

        glfwMakeContextCurrent(_window);
        glfwSwapInterval(0);
        glfwSetErrorCallback(errorcallback);
        glfwSetKeyCallback(_window, keycallback);
        glfwSetCursorPosCallback(_window, mousecallback);
        glfwSetCursorPos(_window, _w/2, _h/2);
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        glewExperimental = 1;
        GLenum glew_status = glewInit();

        if(glew_status != GLEW_OK) {
            glfwTerminate();
            throw backend_error(backend_error_glew);
        }
    }
    Context(const Context &other) = delete;
    ~Context() {
        glfwTerminate();
    }
    Context &operator=(const Context &other) = delete;
    void resetMouse() {
        glfwSetCursorPos(_window, _w/2, _h/2);
    }
    inline bool shouldClose() {
        return glfwWindowShouldClose(_window);
    }
    void close() {
        glfwSetWindowShouldClose(_window, 1);
    }
    inline void ready() {
        _lasttime = glfwGetTime();
        _deltat = 0;
        _fps = 0;
        _fpsc = 0;
        _lastfpstime = _lasttime;
    }
    inline void swap() {
        glfwSwapBuffers(_window);
    }
    inline void start() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void events() {
        auto now = glfwGetTime();
        _deltat = now - _lasttime;
        _lasttime = now;
        if(_lastfpstime + 1 < now) {
            _fps = _fpsc;
            _lastfpstime = now;
            _fpsc = 0;
        }
        ++_fpsc;
        glfwPollEvents();
        idlecallback();
    }
    void meshcontext() {
        if(context == MESH)
            return;
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        context = MESH;
        GLERRORCHECK;
    }
    inline double deltaT() {
        return _deltat;
    }
    inline double deltaTN() {
        return 1.0/_fps;
    }
    inline glm::vec2 getResolution() {
        return glm::vec2(_w, _h);
    }
    inline double getAR() {
        return 1.0 * _w / _h;
    }
    inline unsigned int fps() {
        return _fps;
    }
};
}

#endif
