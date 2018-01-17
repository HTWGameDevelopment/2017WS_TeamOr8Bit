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
#ifndef QE_CACHE_HPP
#define QE_CACHE_HPP

#include<engine/glyphmap.hpp>
#include<engine/programs.hpp>

namespace qe {

    /**
     * \brief Holds frequently used objects
     */
    class Cache {
    public:
        static qe::GlyphmapLatin *glyphlatin; //!< Latin Glyphmap
        static qe::Program *objv1; //!< OBJv1 shader
        static qe::Program *objv2; //!< OBJv2 shader
        static qe::Program *objv3; //!< OBJv3 shader
        static qe::Program *texts; //!< text shader
        static qe::Program *sprite2d; //!< 2D sprite shader
        static qe::Mesh<TEXTG> *meshm; //!< glyph rendering mesh
        static qe::Texture<PNGRGBA, DIFFTEXBIND_GL> *buttont; //!< 2D button texture
        static void __introspect(size_t off);
        /**
         * \brief Destroy all objects
         */
        static void deleteAll() {
            delete glyphlatin;
            delete objv1;
            delete objv2;
            delete objv3;
            delete texts;
            delete sprite2d;
            delete meshm;
            delete buttont;
        }
    };

}

#endif
