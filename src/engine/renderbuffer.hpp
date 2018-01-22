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
#ifndef QE_RENDERBUFFER_HPP
#define QE_RENDERBUFFER_HPP

#include<engine/constants.hpp>

#include<logger.h>

namespace qe {

    // TODO only depth-renderbuffer right now
    class Renderbuffer {
    private:
        GLuint _buffer;
        glm::ivec2 _size;
    public:
        Renderbuffer(glm::ivec2 size): _size(size) {
            glGenRenderbuffers(1, &_buffer);
            bind();
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
        }
        Renderbuffer(const Renderbuffer &other) = delete;
        Renderbuffer(Renderbuffer &&other): _buffer(other._buffer), _size(other._size) {
            other._buffer = 0;
        }
        ~Renderbuffer() {
            glDeleteRenderbuffers(1, &_buffer);
        }
        Renderbuffer &operator=(const Renderbuffer &other) = delete;
        operator GLuint() {
            return _buffer;
        }
        void bind() {
            glBindRenderbuffer(GL_RENDERBUFFER, _buffer);
            GLSERRORCHECK;
        }
    };

}

#endif
