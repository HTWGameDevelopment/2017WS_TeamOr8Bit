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
#include "programs.hpp"

using namespace qe;

Program *qe::mkProgram(std::string vsh, std::string fsh) {
    Program *p = new Program(glCreateProgram());
    GLSERRORCHECK;
    glAttachShader(*p, mkShader<VERTEX>(vsh));
    GLSERRORCHECK;
    glAttachShader(*p, mkShader<FRAGMENT>(fsh));
    GLSERRORCHECK;
    glLinkProgram(*p);
    GLSERRORCHECK;

    GLint status;
    glGetProgramiv(*p, GL_LINK_STATUS, &status);

    if(status == GL_FALSE) {
        glGetProgramiv(*p, GL_INFO_LOG_LENGTH, &status);
        assert(status > 0);
        std::unique_ptr<char[]> m(new char[status]);
        glGetProgramInfoLog(*p, status, NULL, m.get());
        std::string s(m.get());
        throw glshadererror(s);
    }

    return p;
}

std::string qe::getFileContents(std::string p) {
    std::stringbuf buf;
    std::ifstream f(p);
    f.exceptions(std::ifstream::badbit);
    f.get(buf, '\0');

    if(f.bad()) throw loader_error(p, __FILE__, __LINE__);

    return buf.str();
}
