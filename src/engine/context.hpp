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
#include<engine/cache.hpp>

#include<logger.h>

#include<glm/vec2.hpp>

#include<cassert>
#include<chrono>
#include<stdexcept>
#include<string>

namespace qe {

    /**
     * \brief Exception for backend
     */
    struct backend_error: public std::runtime_error {
        /**
         * Construct new backend_error
         *
         * \param s Message
         */
        backend_error(std::string s): runtime_error(s) {}
    };

    /**
     * \brief GLFW key callback
     */
    void keycallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    /**
     * \brief GLFW error callback
     */
    void errorcallback(int code, const char *desc);
    /**
     * \brief GLFW mouse callback
     */
    void mousecallback(GLFWwindow *, double, double);
    /**
     * \brief GLFW mouse button callback
     */
    void mousebuttoncallback(GLFWwindow*, int button, int action, int mods);
    /**
     * \brief Callback right after event handlers after every frame
     */
    void idlecallback();

    /**
     * \brief Class representing the current window and OpenGL context
     */
    class Context {
    private:
        unsigned int _w, _h; //!< Width and height of framebuffer
        std::string _title; //!< Window title
        GLFWwindow *_window; //!< GLFW handle
        enum _context_t {NONE, MESH, TEXT} context = NONE; //!< Current glEnable configuration
        double _lasttime; //!< Timestamp of last frame
        double _deltat; //!< Time difference between last two frames
        unsigned int _fps; //!< Normalized FPS
        double _lastfpstime; //!< Time since last FPS update
        unsigned int _fpsc; //!< Frames since last FPS update
    public:
        /**
         * Construct new OpenGL context and window
         *
         * \param title Window title
         * \param w Width in px
         * \param h Height in px
         */
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
            glfwSwapInterval(qe::VSYNC());
            qe::VSYNC() << [this](int old, int upd) {
                glfwSwapInterval(upd);
            };
            glfwSetErrorCallback(errorcallback);
            glfwSetKeyCallback(_window, keycallback);
            glfwSetMouseButtonCallback(_window, mousebuttoncallback);
            glfwSetCursorPosCallback(_window, mousecallback);
            glfwSetCursorPos(_window, 0, 0);
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glCullFace(GL_BACK);

            int nw, nh;
            glfwGetFramebufferSize(_window, &nw, &nh);
            _w = w;
            _h = h;
        }
        Context(const Context &other) = delete;
        ~Context() {
            qe::Cache::deleteAll();
            glfwTerminate();
        }
        Context &operator=(const Context &other) = delete;
        /**
         * \brief Center mouse
         */
        void resetMouse() {
            glfwSetCursorPos(_window, 0, 0);
        }
        /**
         * \brief Return mouse position
         */
        glm::dvec2 getMousePos() {
            glm::dvec2 r;
            glfwGetCursorPos(_window, &r.x, &r.y);
            return r;
        }
        /**
         * \brief Get coordinate of screen center
         */
        glm::dvec2 getCenterCoordinate() {
            return glm::dvec2(0.5 * _w, 0.5 * _h);
        }
        /**
         * \brief Convert absolute coordinates (in pixels) to window coordinates (-1,-1 to 1,1)
         */
        glm::dvec2 absToRel(glm::dvec2 in) {
            return glm::dvec2(-1.0 + 2.0 * in.x / _w, -1.0 + 2.0 * in.y / _h);
        }
        /**
         * \brief true if close() call or window was closed by user
         */
        inline bool shouldClose() {
            return glfwWindowShouldClose(_window);
        }
        /**
         * \brief Close context after this frame
         */
        void close() {
            glfwSetWindowShouldClose(_window, 1);
        }
        /**
         * \brief Initialize timers
         */
        inline void ready() {
            _lasttime = glfwGetTime();
            _deltat = 0;
            _fps = 0;
            _fpsc = 0;
            _lastfpstime = _lasttime;
        }
        /**
         * \brief Swap buffers
         */
        inline void swap() {
            glfwSwapBuffers(_window);
        }
        /**
         * \brief Start of frame
         */
        inline void start() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        /**
         * \brief Handle GLFW events and call idlecallback
         */
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
        /**
         * \brief Display cursor
         */
        void displayCursor(glm::dvec2 pos = glm::dvec2(0, 0)) {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPos(_window, pos.x, pos.y);
        }
        /**
         * \brief Hide cursor (FPS mode)
         */
        void hideCursor() {
            glfwSetCursorPos(_window, 0, 0);
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        /**
         * \brief Handle GLFW events. blocks until event received
         */
        void waitEvents() {
            glfwWaitEvents();
        }
        /**
         * \brief Set glEnable config to MESH rendering
         */
        void meshcontext() {
            if(context == MESH)
                return;

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glDisable(GL_BLEND);
            context = MESH;
            GLERRORCHECK;
        }
        /**
         * \brief Set glEnable config to TEXT rendering
         */
        void textcontext() {
            if(context == TEXT)
                return;

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            context = TEXT;
            GLERRORCHECK;
        }
        /**
         * \brief Return time since last frame
         */
        inline double deltaT() {
            return _deltat;
        }
        /**
         * \brief deltaT normalized over a second
         */
        inline double deltaTN() {
            return 1.0 / _fps;
        }
        /**
         * \brief Get framebuffer resolution
         */
        inline glm::vec2 getResolution() {
            return glm::vec2(_w, _h);
        }
        /**
         * \brief Get aspect ratio
         */
        inline double getAR() {
            return 1.0 * _w / _h;
        }
        /**
         * \brief Return frames per second
         */
        inline unsigned int fps() {
            return _fps;
        }
        /**
         * \brief Return width of framebuffer
         */
        inline unsigned int width() {
            return _w;
        }
        /**
         * \brief Return height of framebuffer
         */
        inline unsigned int height() {
            return _h;
        }
    };
}

#endif
