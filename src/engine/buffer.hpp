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

namespace qe {

    /**
     * \brief Generic OpenGL buffer implementation
     */
    template<GLenum target, typename T = uint8_t>
    class Buffer {
    private:
        GLuint _buffer;
        size_t _size;
        T *_map;
    public:
        Buffer(): _size(0), _map(nullptr) {
            glGenBuffers(1, &_buffer);
        }
        Buffer(const Buffer &other) = delete;
        Buffer(Buffer &&other): _buffer(other._buffer), _size(other._size), _map(other._map) {
            other._map = nullptr;
            other._buffer = 0;
            other._size = 0;
        }
        ~Buffer() {
            glDeleteBuffers(1, &_buffer);
            delete [] _map;
        }
        Buffer &operator=(const Buffer &other) = delete;
        size_t size() {
            return _size;
        }
        T *ptr() {
            return _map;
        }
        operator GLuint() {
            return _buffer;
        }
        void bind() {
            glBindBuffer(target, _buffer);
            GLSERRORCHECK;
        }
        template<GLenum usage>
        void data(void *data, size_t size, bool caching = false) {
            bind();
            glBufferData(target, size, data, usage);
            GLERRORCHECK;
            _size = size;
            if(caching) {
                if(_map) delete [] _map;
                _map = new T[size / sizeof(T)];
                if(data) {
                    memcpy(_map, data, size);
                }
            }
        }
        void bindTo(GLuint index) {
            glBindBufferBase(GL_UNIFORM_BUFFER, index, _buffer);
            GLSERRORCHECK;
        }
        void update() {
            assert(_map);
            bind();
            glBufferSubData(target, 0, _size, _map);
            GLSERRORCHECK;
        }
        T &operator[](size_t i) {
            assert(_map);
            return ((T*)_map)[i];
        }
    };

}

#endif
