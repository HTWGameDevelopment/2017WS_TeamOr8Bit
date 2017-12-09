#ifndef _FONT_HPP
#define _FONT_HPP

#include<config.h>

#ifdef HAS_FREETYPE
#include<font/font_linux.hpp>
#else
#include<font/font_win.hpp>
#endif

#endif
