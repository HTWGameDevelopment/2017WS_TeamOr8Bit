// Copyright (c) 2017-2018 Fabian Stiewitz
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
GlyphmapLatin::GlyphmapLatin(std::string bakepath, FT_Face face, size_t height, glm::ivec2 res): _res(res), _path(bakepath) {
    initialize(face, height);
}

void GlyphmapLatin::initialize(FT_Face face, size_t height) {
    Loader<TEXTG> loader(texlength);

    FT_Set_Pixel_Sizes(face, 0, height);
    size_t x = 0, y = 0;
    size_t max_y = 0;
    _linespace = (face->size->metrics.height) >> 6;
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

savegame::SaveGame &&glyphmap_migrate(savegame::SaveGame &&sg, unsigned int v, unsigned int wanted) {
    if(wanted == 1 && v == 0) {
        uint32_t linespace_default = 32;
        sg.storeDataBlock("linespace", 4, (unsigned char*)&linespace_default);
        sg.save();
    }
    return std::move(sg);
}

GlyphmapLatin::GlyphmapLatin(std::string path, glm::ivec2 res): _res(res) {
    // load file
    auto sg = savegame::load<1, 1>(path, "GlyphmapLatin", glyphmap_migrate);
    // fill _metrics
    auto metrics = sg.getDataBlock("metrics");
    memcpy(_metrics.data(), metrics.data, sizeof(fontmetrics) * capacity);
    // get linespace
    auto linespace = sg.getDataBlock("linespace");
    uint32_t ls;
    memcpy(&ls, linespace.data, 4);
    _linespace = ls;
    // create texture
    auto texture = sg.getDataBlock("texture");
    _glyphmap.reset(new Texture<TEXTG, FONTMAPBIND_GL>(Loader<TEXTG>(texture)));
    _glyphmap->bindTo();
}

void GlyphmapLatin::bake() {
    // load file
    auto sg = savegame::load<1, 1>(std::string(_path), "GlyphmapLatin", glyphmap_migrate);
    // fill _metrics
    sg.storeDataBlock("metrics", sizeof(fontmetrics) * capacity, (unsigned char *)_metrics.data());
    // fill texture
    auto &l = _glyphmap->getLoader();
    sg.storeDataBlock("texture", l.size(), l.parse());
    uint32_t linespace = _linespace;
    sg.storeDataBlock("linespace", 4, (unsigned char*)&linespace);
    sg.save();
}

void GlyphmapLatin::__introspect(size_t off) {
    std::cout << std::string(off, ' ') << "GlyphmapLatin" << std::endl;
    std::string os(off + 2, ' ');
    std::cout << os << "path=" << _path << std::endl;
    std::cout << os << "res=" << GV2TOSTR(_res) << std::endl;
    _glyphmap->__introspect(off + 2);
    std::cout << os << "offsets=" << std::endl;
    for(size_t i = 0; i < _metrics.size(); ++i) {
        std::cout << os << "  ";
        printf("%04zu %04zu %04zu %04zu %04zu %04zi %04zi %04zi %04zi\n",
            i,
            _metrics[i].x,
            _metrics[i].y,
            _metrics[i].w,
            _metrics[i].h,
            _metrics[i].off_x,
            _metrics[i].off_y,
            _metrics[i].adv_x,
            _metrics[i].adv_y);
    }
}
