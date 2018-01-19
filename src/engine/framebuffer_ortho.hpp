// Copyright (c) 2017-2018 Fabian Stiewitz
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
#ifndef QE_FRAMEBUFFER_ORTHO_HPP
#define QE_FRAMEBUFFER_ORTHO_HPP

#include<engine/framebuffer.hpp>
#include<engine/textures.hpp>

namespace qe {

    class FramebufferOrtho: public Framebuffer {
    private:
        Texture<TEXD, DIFFTEXBIND_GL> _depth;
        Texture<RGB, DIFFTEXBIND_GL> _color;
    public:
        FramebufferOrtho(glm::ivec2 size): Framebuffer(), _depth(Loader<TEXD>(size)), _color(Loader<RGB>(size)) {
            attach(GL_COLOR_ATTACHMENT0, _color);
            attach(GL_DEPTH_ATTACHMENT, _depth);
            valid();
        }
        virtual ~FramebufferOrtho() {}
        Texture<RGB, DIFFTEXBIND_GL> &get_color() {
            return _color;
        }
    };

}

#endif
