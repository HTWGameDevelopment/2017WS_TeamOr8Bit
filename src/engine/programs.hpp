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
#include<engine/glyphmap.hpp>

#include<logger.h>

#include<glm/glm.hpp>

#include<fstream>
#include<memory>
#include<string>
#include<sstream>

namespace qe {

    typedef GLuint shader_t;

    /**
     * \brief An OpenGL program
     */
    class Program {
    private:
        GLuint _program; //!< OpenGL handle
        bool _initialized; //!< Ready to use?
    public:
        /**
         * Default constructor
         */
        Program(): _initialized(false) {}
        /**
         * \brief Construct from OpenGL handle
         */
        Program(GLuint p): _program(p), _initialized(true) {}
        Program(const Program &other) = delete;
        Program(Program &&other) {
            _initialized = other._initialized;
            _program = other._program;
            other._initialized = false;
        }
        /**
         * \brief Destroy OpenGL handle
         */
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
        /**
         * \brief Cast to OpenGL handle
         */
        operator GLuint() {
            return _program;
        }
        /**
         * \brief Use program
         */
        void use() {
            assert(_initialized);
            glUseProgram(_program);
            GLSERRORCHECK;
        }
        /**
         * \brief Set uniform to 4x4 matrix
         */
        template<flag_t binding> void setUniform(glm::mat4 m) {
            glUniformMatrix4fv(binding, 1, GL_FALSE, (float *)(&m));
            GLSERRORCHECK;
        }
        /**
         * \brief Set uniform to int
         */
        template<flag_t binding> void setUniform(int i) {
            glUniform1i(binding, i);
            GLSERRORCHECK;
        }
        /**
         * \brief Set uniform to 3-vector
         */
        template<flag_t binding> void setUniform(glm::vec3 v) {
            glUniform3fv(binding, 1, (float *)&v);
            GLSERRORCHECK;
        }
        template<flag_t binding> void setUniform(glm::vec2 pos) {
            glUniform2fv(binding, 1, (float *)&pos);
            GLSERRORCHECK;
        }
        template<flag_t binding> void setUniform(glm::vec4 pos) {
            glUniform4fv(binding, 1, (float *)&pos);
            GLSERRORCHECK;
        }
        void __introspect(size_t off) {
            std::cout << std::string(off, ' ') << "Program[" << _initialized << "]" << std::endl;
        }
    };

    /**
     * \brief Get file contents from path
     */
    std::string getFileContents(std::string p);

    template<flag_t type> constexpr GLenum _createshadercall();

    template<> constexpr GLenum _createshadercall<VERTEX>() {
        return GL_VERTEX_SHADER;
    }
    template<> constexpr GLenum _createshadercall<FRAGMENT>() {
        return GL_FRAGMENT_SHADER;
    }

    /**
     * \brief Compile shader from source code string
     */
    template<flag_t type>
    shader_t compileShader(std::string c) {
        shader_t s = glCreateShader(_createshadercall<type>());
        GLSERRORCHECK;
        const char *t[1];
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

    /**
     * \brief Compile shader from file path
     */
    template<flag_t type>
    shader_t mkShader(std::string file) {
        std::string path = getPath(file);
        std::string contents = getFileContents(path);
        return compileShader<type>(contents);
    }

    /**
     * \brief Compile program from vertex and fragment shader file paths
     */
    Program *mkProgram(std::string vsh, std::string fsh);
}

#endif
