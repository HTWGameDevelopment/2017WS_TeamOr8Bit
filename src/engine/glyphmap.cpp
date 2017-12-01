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
#include "glyphmap.hpp"

#include<savegame/savegame.hpp>

using namespace qe;

#ifdef HAS_FREETYPE
GlyphmapLatin::GlyphmapLatin(FT_Face face, size_t height, glm::ivec2 res): _res(res) {
    initialize(face, height);
}

void GlyphmapLatin::initialize(FT_Face face, size_t height) {
    Loader<TEXTG> loader(texlength);

    FT_Set_Pixel_Sizes(face, 0, height);
    size_t x = 0, y = 0;
    size_t max_y = 0;

    for(size_t i = 0; i < capacity; ++i) {
        FT_UInt glyph_index = FT_Get_Char_Index(face, i);
        if(FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) continue;
        if(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) continue;

        if(x + face->glyph->bitmap.width > texlength) { // new line
            x = 0;
            y += max_y;
            max_y = 0;
            if(y > texlength) throw loader_error(std::string("glyphmap too small for ") + std::to_string(capacity) + " glyphs at size "  + std::to_string(height), __FILE__, __LINE__);
        }

        _metrics[i] = fontmetrics {
            x,
            y,
            face->glyph->bitmap.rows,
            face->glyph->bitmap.width,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            (int)(face->glyph->advance.x >> 6),
            (int)(face->glyph->advance.y >> 6)
        };

        loader.setRect(x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

        x += face->glyph->bitmap.width;
        if(max_y < face->glyph->bitmap.rows) max_y = face->glyph->bitmap.rows;
    }

    _glyphmap.reset(new Texture<TEXTG, FONTMAPBIND_GL>(std::move(loader)));
    _glyphmap->bindTo();
}
#endif

GlyphmapLatin::GlyphmapLatin(const char* path, glm::ivec2 res): _res(res) {
    // load file
    auto sg = savegame::load<1, 0>(std::string(path), "GlyphmapLatin");
    // fill _metrics
    auto metrics = sg.getDataBlock("metrics");
    memcpy(_metrics.data(), metrics.data.get(), sizeof(fontmetrics) * capacity);
    // create texture
    auto texture = sg.getDataBlock("texture");
    _glyphmap.reset(new Texture<TEXTG, FONTMAPBIND_GL>(Loader<TEXTG>(texture)));
    _glyphmap->bindTo();
}
