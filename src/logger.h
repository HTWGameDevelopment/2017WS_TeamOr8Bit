#ifndef LOGGER_H
#define LOGGER_H

#include<stdio.h>
#include<assert.h>
#include<glad/glad.h>

#ifdef __cplusplus
#include<string>
#include<iostream>
inline std::string cut(const char* f) {
    return std::string(f).erase(0, std::string(f).find("src/") + 4);
}
#define GV4TOSTR(t) "vec4(" << t.x << "," << t.y << "," << t.z << ", " << t.w << ")"
#define GV3TOSTR(t) "vec3(" << t.x << "," << t.y << "," << t.z << ")"
#define GV2TOSTR(t) "vec2(" << t.x << "," << t.y << ")"
extern GLenum _logger_e;
#define GLERRORCHECK if((_logger_e = glGetError()) != GL_NO_ERROR) throw_glerror(_logger_e, __FILE__, __LINE__);
#ifndef NDEBUG
#define GERR(x)  std::cerr <<  "GERROR " << x << " (" << cut(__FILE__) << ":" << __LINE__ << ")" << std::endl;
#define GWARN(x)  std::cerr << "GWARN  " << x << " (" << cut(__FILE__) << ":" << __LINE__ << ")" << std::endl;
#define GINFO(x)  std::cerr << "GINFO  " << x << " (" << cut(__FILE__) << ":" << __LINE__ << ")" << std::endl;
#define GDBG(x)  std::cerr <<  "GDBG   " << x << " (" << cut(__FILE__) << ":" << __LINE__ << ")" << std::endl;
#define GLSERRORCHECK if((_logger_e = glGetError()) != GL_NO_ERROR) throw_glerror(_logger_e, __FILE__, __LINE__);
#else
#define GERR(x)  std::cerr << "GERROR " << x << std::endl;
#define GWARN(x)  std::cerr << "GWARN " << x << std::endl;
#define GINFO(x)  std::cerr << "GINFO " << x << std::endl;
#define GDBG(x)
#define GLSERRORCHECK
#endif
struct glerror: public std::runtime_error {
    glerror(std::string s): runtime_error(s) {}
};
struct glshadererror: public std::runtime_error {
    glshadererror(std::string s): runtime_error(s) {}
};
const char *getErrorString(GLenum error);
void throw_glerror(GLenum error, const char *file, int line);
#else
#ifndef NDEBUG
#define GERR(x)  fprintf(stderr, "GERROR %s (%s:%s)\n", x, __FILE__, __LINE__);
#define GWARN(x) fprintf(stderr, "GWARN  %s (%s:%s)\n", x, __FILE__, __LINE__);
#define GLOG(x)  fprintf(stderr, "GLOG   %s (%s:%s)\n", x, __FILE__, __LINE__);
#else
#define GERR(x)  fprintf(stderr, "GERROR %s\n", x);
#define GWARN(x) fprintf(stderr, "GWARN  %s\n", x);
#define GLOG(x)  fprintf(stderr, "GLOG   %s\n", x);
#endif
#endif

#endif
// vim: noet
