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
#ifndef QE_FRAMEBUFFER_FIRST_PASS_HPP
#define QE_FRAMEBUFFER_FIRST_PASS_HPP

#include<engine/framebuffer.hpp>
#include<engine/renderbuffer.hpp>
#include<engine/textures.hpp>

namespace qe {

    class FramebufferFirstPass: public Framebuffer {
    private:
        Renderbuffer _depth;
        Texture<RG32UI, DIFFTEXBIND_GL> _tileno;
    public:
        FramebufferFirstPass(glm::ivec2 size): Framebuffer(), _depth(size), _tileno(Loader<RG32UI>(size)) {
            attachTexture(GL_COLOR_ATTACHMENT0, _tileno);
            attachRenderbuffer(GL_DEPTH_ATTACHMENT, _depth);
            valid();
        }
        virtual ~FramebufferFirstPass() {}
        Texture<RG32UI, DIFFTEXBIND_GL> &get_tileno() {
            return _tileno;
        }
        void clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    };

}

#endif
