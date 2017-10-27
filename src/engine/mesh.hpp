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
#ifndef QE_MESH_HPP
#define QE_MESH_HPP

#include<engine/buffer.hpp>
#include<engine/loader.hpp>

#include<logger.h>

#include<type_traits>
#include<memory>

namespace qe {

template<flag_t T> class Mesh {
private:
    Buffer<GL_ARRAY_BUFFER> _buffer;
    GLuint _vao;
    size_t _size;
    void initVAO() {
        if(T == OBJV1) return initVAOAsOBJV1();
        assert(false);
    }
    void initVAOAsOBJV1() {
        glGenVertexArrays(1, &_vao);
        GLSERRORCHECK;
        glBindVertexArray(_vao);
        GLSERRORCHECK;
        glEnableVertexAttribArray(0);
        GLSERRORCHECK;
        _buffer.bind();
        glVertexAttribPointer(
            (GLuint)0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        GLSERRORCHECK;
        glBindVertexArray(0);
    }
public:
    Mesh(Loader<T> &&l): _size(l.size()) {
        _buffer.data<GL_STATIC_DRAW>(l.parse().data(), l.elementSize() * l.size());
        initVAO();
    }
    Mesh(const Mesh<T> &other) = delete;
    Mesh(Mesh<T> &&other) = delete;
    ~Mesh() {
        glDeleteVertexArrays(1, &_vao);
    }
    Mesh<T> &operator=(const Mesh<T> &other) = delete;
    Mesh<T> &operator=(Mesh<T> &&other) = delete;
    void render() {
        glBindVertexArray(_vao);
        GLSERRORCHECK;
        glDrawArrays(GL_TRIANGLES, 0, _size);
        GLSERRORCHECK;
    }
};

}

#endif
