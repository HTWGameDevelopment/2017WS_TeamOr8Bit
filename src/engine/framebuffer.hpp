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
#ifndef QE_FRAMEBUFFER_HPP
#define QE_FRAMEBUFFER_HPP

#include<engine/constants.hpp>

namespace qe {

    class Framebuffer {
    private:
        GLuint _fb;
    protected:
        void attach(GLenum attachment, GLuint texture) {
            bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
        }
    public:
        Framebuffer() {
            glGenFramebuffers(1, &_fb);
            GLSERRORCHECK;
        }
        Framebuffer(const Framebuffer &other) = delete;
        Framebuffer(Framebuffer &&other): _fb(other._fb) {
            other._fb = 0;
        }
        virtual ~Framebuffer() {
            glDeleteFramebuffers(1, &_fb);
            GLSERRORCHECK;
        }
        virtual Framebuffer &operator=(const Framebuffer &other) = delete;
        virtual Framebuffer &operator=(Framebuffer &&other) {
            _fb = other._fb;
            other._fb = 0;
            return *this;
        }
        void bind() {
            glBindFramebuffer(GL_FRAMEBUFFER, _fb);
            GLSERRORCHECK;
        }
        static void unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        bool valid() {
            bind();
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    GERR("FBO.valid: Incomplete Attachment");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    GERR("FBO.valid: No Attachments");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    GERR("FBO.valid: Invalid readBuffer");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    GERR("FBO.valid: Incomplete Multisample");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                    GERR("FBO.valid: Incomplete Layer Targets");
                    break;
                }
                return false;
            }
            unbind();
            return true;
        }
    };

}

#endif
