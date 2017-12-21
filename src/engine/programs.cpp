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
