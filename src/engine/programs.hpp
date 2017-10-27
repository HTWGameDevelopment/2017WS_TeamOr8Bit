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
#ifndef QE_PROGRAMS_HPP
#define QE_PROGRAMS_HPP

#include<engine/constants.hpp>
#include<engine/paths.hpp>
#include<engine/loader.hpp>

#include<logger.h>

#include<glm/glm.hpp>

#include<fstream>
#include<memory>
#include<string>
#include<sstream>

namespace qe {

typedef GLuint shader_t;

class Program {
private:
    GLuint _program;
    bool _initialized;
public:
    Program(): _initialized(false) {}
    Program(GLuint p): _program(p), _initialized(true) {}
    Program(const Program &other) = delete;
    Program(Program &&other) {
        _initialized = other._initialized;
        _program = other._program;
        other._initialized = false;
    }
    ~Program() {
        if(_initialized) glDeleteProgram(_program);
    }
    Program &operator=(const Program &other) = delete;
    Program &operator=(Program &&other) {
        if(_initialized) glDeleteProgram(_program);
        _initialized = other._initialized;
        _program = other._program;
        other._initialized = false;
        return *this;
    }
    operator GLuint() {
        return _program;
    }
    void use() {
        assert(_initialized);
        glUseProgram(_program);
        GLSERRORCHECK;
    }
    template<flag_t binding>
    void setUniform(glm::mat4 &m) {
        glUniformMatrix4fv(binding, 1, GL_FALSE, (float*)(&m));
    }
};

std::string getFileContents(std::string p) {
    std::stringbuf buf;
    std::ifstream f(p);
    f.exceptions(std::ifstream::badbit);
    f.get(buf, '\0');
    if(f.bad()) throw loader_error(p, __FILE__, __LINE__);
    return buf.str();
}

template<flag_t type> constexpr GLenum _createshadercall();

template<> constexpr GLenum _createshadercall<VERTEX>() {
    return GL_VERTEX_SHADER;
}
template<> constexpr GLenum _createshadercall<FRAGMENT>() {
    return GL_FRAGMENT_SHADER;
}

template<flag_t type>
shader_t compileShader(std::string c) {
    shader_t s = glCreateShader(_createshadercall<type>());
    GLSERRORCHECK;
    const char* t[1];
    t[0] = c.c_str();
    glShaderSource(s, 1, t, NULL);
    GLSERRORCHECK;
    glCompileShader(s);
    GLSERRORCHECK;

    GLint p;
    glGetShaderiv(s, GL_COMPILE_STATUS, &p);

    if(p == GL_FALSE) {
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &p);
        assert(p > 0);
        std::unique_ptr<char[]> m(new char[p]);
        glGetShaderInfoLog(s, p, NULL, m.get());
        std::string st(m.get());
        throw glshadererror(st);
    }
    return s;
}

template<flag_t type>
shader_t mkShader(std::string file) {
    std::string path = getPath(file);
    std::string contents = getFileContents(path);
    return compileShader<type>(contents);
}

Program mkProgram(std::string vsh, std::string fsh) {
    Program p(glCreateProgram());
    GLSERRORCHECK;
    glAttachShader(p, mkShader<VERTEX>(vsh));
    GLSERRORCHECK;
    glAttachShader(p, mkShader<FRAGMENT>(fsh));
    GLSERRORCHECK;
    glLinkProgram(p);
    GLSERRORCHECK;

    GLint status;
    glGetProgramiv(p, GL_LINK_STATUS, &status);

    if(status == GL_FALSE) {
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &status);
        assert(status > 0);
        std::unique_ptr<char[]> m(new char[status]);
        glGetProgramInfoLog(p, status, NULL, m.get());
        std::string s(m.get());
        throw glshadererror(s);
    }
    return p;
}

}

#endif
