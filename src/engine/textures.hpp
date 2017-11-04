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
#ifndef QE_TEXTURE_HPP
#define QE_TEXTURE_HPP

#include<engine/loader.hpp>

#include<logger.h>

namespace qe {
    struct GLFlagSet {
        GLenum target;
        GLint internalFormat;
        GLenum format;
        GLenum type;
    };

    const GLFlagSet getGLFlagSet(flag_t flag) {
        switch(flag) {
            case qe::PNGRGBA: return qe::GLFlagSet {GL_TEXTURE_2D, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE};
            default: assert(false);
        }
    }

    template<flag_t T> class Texture {
    private:
        Loader<T> _source;
        GLuint _texture;
        const GLFlagSet _gl = getGLFlagSet(T);
        void initTexture() {
            if(T == PNGRGBA) return initTextureAsRGBA();
            assert(false);
        }
        void initTextureAsRGBA() {
            glGenTextures(1, &_texture); GLSERRORCHECK;
            glBindTexture(_gl.target, _texture); GLSERRORCHECK;
            glTexImage2D(_gl.target,
                0,
                _gl.internalFormat,
                _source.width(),
                _source.height(),
                0,
                _gl.format,
                _gl.type,
                _source.parse());
            glTexParameteri(_gl.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(_gl.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(_gl.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(_gl.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GLERRORCHECK;
        }
    public:
        Texture(Loader<T> &&l): _source(std::move(l)) {
            initTexture();
        }
        Texture(const Texture<T> &other) = delete;
        Texture(Texture<T> &&other) = delete;
        ~Texture() {
            glDeleteTextures(1, &_texture);
        }
        Texture<T> &operator=(const Texture<T> &other) = delete;
        Texture<T> &operator=(Texture<T> &&other) = delete;
        void bind() {
            glBindTexture(_gl.target, _texture); GLERRORCHECK;
        }
        template<GLenum target>
        void bindTo() {
            glActiveTexture(target); GLERRORCHECK;
            bind();
        }
    };

}

#endif