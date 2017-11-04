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
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<png.h>

enum _qe_read_png_error_t { READ_STRUCT, INFO_STRUCT, READ_IMAGE };
enum _qe_read_png_error_t _qe_read_png_error;

unsigned char *_qe_read_png(FILE *fd, size_t *size, size_t *_width, size_t *_height) {
    assert(fd);
    assert(size);
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
        _qe_read_png_error = READ_STRUCT;
        return NULL;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        _qe_read_png_error = INFO_STRUCT;
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        return NULL;
    }
    png_infop end_info = png_create_info_struct(png_ptr);
    if(!end_info) {
        _qe_read_png_error = INFO_STRUCT;
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        return NULL;
    }

    png_init_io(png_ptr, fd);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    png_byte color = png_get_color_type(png_ptr, info_ptr);
    png_byte depth = png_get_bit_depth(png_ptr, info_ptr);

    switch(color) {
        case PNG_COLOR_TYPE_PALETTE: png_set_palette_to_rgb(png_ptr); break;
        case PNG_COLOR_TYPE_GRAY: if(depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr); png_set_gray_to_rgb(png_ptr); break;
        case PNG_COLOR_TYPE_RGB: png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER); break;
    }

    if(depth == 16) png_set_strip_16(png_ptr);
    else if(depth < 8) png_set_packing(png_ptr);

    assert(png_get_rowbytes(png_ptr, info_ptr) == width * 4);

    png_bytep *rows;

    if(setjmp(png_jmpbuf(png_ptr))) {
        for(int i = 0; i < height; ++i) {
            free(rows[i]);
        }
        free(rows);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        _qe_read_png_error = READ_IMAGE;
        return NULL;
    }

    rows = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for(int i = 0; i < height; ++i)
        rows[i] = (png_byte*) malloc(width * 4);

    png_read_image(png_ptr, rows);

    unsigned char* buffer = (unsigned char*) malloc(width * height * 4);
    for(int i = 0; i < height; ++i)
        memcpy(buffer + i * width * 4, rows[i], width * 4);

    for(int i = 0; i < height; ++i) {
        free(rows[i]);
    }
    free(rows);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    *size = width * height;
    *_width = width;
    *_height = height;
    return buffer;
}
