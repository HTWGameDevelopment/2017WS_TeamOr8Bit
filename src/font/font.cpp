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
#include<config.h>
#ifdef HAS_FREETYPE
#include "font.hpp"

using namespace font;

Font *Font::_instance;

Font *Font::get() {
    assert(_instance);
    return _instance;
}

Font *Font::get(std::string font) {
    if(_instance) delete _instance;
    _instance = new Font(font);
    return get();
}

Font::Font(std::string font) {
    if(FT_Init_FreeType(&_library)) throw font_error("FT_Init_FreeType", __FILE__, __LINE__);
    auto error = FT_New_Face(_library, font.c_str(), 0, &_face);
    if(error == FT_Err_Unknown_File_Format) throw font_error("FT Unknown file format", __FILE__, __LINE__);
    else if(error != 0) throw font_error(std::string("FT_New_Face ") + std::to_string(error), __FILE__, __LINE__);
}

Font::~Font() {
    FT_Done_Face(_face);
    FT_Done_FreeType(_library);
}
#endif
