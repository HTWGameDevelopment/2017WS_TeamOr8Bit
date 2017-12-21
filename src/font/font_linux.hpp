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
#ifndef _FONT_LINUX_HPP
#define _FONT_LINUX_HPP

#include<string>
#include<stdexcept>
#include<assert.h>

#include<ft2build.h>
#include FT_FREETYPE_H

namespace font {

    /**
     * \brief Freetype font
     */
    class Font {
    private:
        static Font *_instance; //!< Font instance
        FT_Library _library; //!< FT_Library handle
        FT_Face _face; //!< FT_Face handle
        std::string _bpath; //!< Baked font path
        Font(std::string font);
    public:
        ~Font();
        static Font *get();
        static Font *get(std::string font);
        FT_Face face() {
            return _face;
        }
        std::string bpath() {
            return _bpath;
        }
    };

    /**
    * \brief Exception from Font
    */
    struct font_error: public std::runtime_error {
        /**
        * \brief Constructs a new font_error
        *
        * \param s Message
        * \param file File path
        * \param line Source line
        */
        font_error(std::string s, const char *file, int line): runtime_error(std::string(file) + ":" + std::to_string(line) + ": Failed " + s) {}
    };

}

#endif
