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
#ifndef QE_GLYPHMAP_HPP
#define QE_GLYPHMAP_HPP

#include<engine/textures.hpp>

namespace qe {

    template<> class Loader<TEXTG> {
    private:
        std::unique_ptr<unsigned char[]> _pixels;
        size_t _size;
        size_t _length;
    public:
        Loader(size_t l): _size(l * l), _length(l), _pixels(new unsigned char[_size]) {}
        unsigned char* parse() {return _pixels.get();}
        size_t size() {return _size;}
        size_t width() {return _length;}
        size_t height() {return _length;}
        unsigned int elementSize() {return 1;}
    };

    class Glyphmap {
    private:
        Texture<TEXTG> _glyphmap;
    public:
        Glyphmap();
        Glyphmap(const Glyphmap &other) = delete;
        Glyphmap(Glyphmap &&other) = delete;
        Glyphmap &operator=(const Glyphmap &other) = delete;
        Glyphmap &operator=(Glyphmap &&other) = delete;
    };

}

#endif
