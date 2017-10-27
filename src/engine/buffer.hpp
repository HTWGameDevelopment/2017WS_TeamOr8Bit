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
#ifndef QE_BUFFER_HPP
#define QE_BUFFER_HPP

#include<engine/constants.hpp>

#include<logger.h>

#include<GL/glew.h>

namespace qe {

template<GLenum target>
class Buffer {
private:
    GLuint _buffer;
    size_t _size;
public:
    Buffer(): _size(0) {
        glGenBuffers(1, &_buffer);
    }
    Buffer(const Buffer &other) = delete;
    Buffer(Buffer &&other) = default;
    ~Buffer() {
        glDeleteBuffers(1, &_buffer);
    }
    Buffer &operator=(const Buffer &other) = delete;
    Buffer &operator=(Buffer &&other) = default;
    operator GLuint() {
        return _buffer;
    }
    void bind() {
        glBindBuffer(target, _buffer);
        GLSERRORCHECK;
    }
    template<GLenum usage>
    void data(void *data, size_t size) {
        bind();
        glBufferData(target, size, data, usage);
        GLERRORCHECK;
        _size = size;
    }
};

}

#endif
