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
#include "screenshot.hpp"

#include "loader.hpp"

#include "../logger.h"

void qe::screenshot(std::string path, unsigned int w, unsigned int h) {
    assert(w != 0);
    assert(h != 0);
    std::unique_ptr<std::FILE, decltype(&std::fclose)> fd(fopen(path.c_str(), "w"), &std::fclose);

    if(!fd) throw writer_error(std::string(path), __FILE__, __LINE__);

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(!png_ptr)
        throw writer_error(std::string(path) + " (png write struct)", __FILE__, __LINE__);

    png_infop info_ptr = png_create_info_struct(png_ptr);

    if(!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        throw writer_error(std::string(path) + " (png info struct)", __FILE__, __LINE__);
    }

    png_init_io(png_ptr, fd.get());

    if(setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        throw writer_error(std::string(path) + " (png write image)", __FILE__, __LINE__);
    }

    png_set_IHDR(png_ptr,
                 info_ptr,
                 w, h, 8,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    std::unique_ptr<unsigned char[]> pixels(new unsigned char[w * h * 3]);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels.get());
    GLERRORCHECK;

    std::unique_ptr<png_bytep[]> rows(new png_bytep[h]);

    for(unsigned int i = 0; i < h; ++i)
        rows[i] = pixels.get() + (h - 1 - i) * w * 3;

    png_write_image(png_ptr, rows.get());
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}
