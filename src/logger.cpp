#include "logger.h"

GLenum _logger_e;

const char* getErrorString(GLenum error) {
    switch(error) {
        case GL_NO_ERROR: return "no error";
        case GL_INVALID_ENUM: return "invalid enum";
        case GL_INVALID_VALUE: return "invalid value";
        case GL_INVALID_OPERATION: return "invalid operation";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "invalid framebuffer operation";
        case GL_OUT_OF_MEMORY: return "out of memory";
        case GL_STACK_UNDERFLOW: return "stack undeflow";
        case GL_STACK_OVERFLOW: return "stack overflow";
        default: return "unknown error";
    }
}

void throw_glerror(GLenum error, const char* file, int line) {
    std::string r(file);
    r += ":";
    r += std::to_string(line);
    r += ": ";
    r += (const char*)getErrorString(error);
    throw glerror(r);
}
