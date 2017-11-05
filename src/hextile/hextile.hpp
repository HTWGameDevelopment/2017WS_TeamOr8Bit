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
#ifndef HEXTILE_HPP
#define HEXTILE_HPP

#include<memory>
#include<vector>

namespace hextile {

    struct hexpoint_t {
        size_t x;
        size_t y;
    };

    template<typename T>
    struct is_hextile_tile {
        template<typename U, U& (U::*)(hexpoint_t)> struct yes {};
        template<typename U> static char test(yes<U, &U::operator=>*);
        template<typename U> static int test(...);
        static const bool value = sizeof(test<T>(0,0)) == sizeof(char);
    };

    template<typename T, typename = std::enable_if<is_hextile_tile<T>::value>>
    class HexTile {
    public:
        typedef T tile_type;
        struct col_type {
            std::vector<tile_type> rows;
            col_type(size_t i): rows(i, tile_type()) {}
            tile_type &operator[](size_t i) {return rows[i];}
        };
    private:
        size_t _x;
        size_t _y;
        std::vector<col_type> _data;
    public:
        HexTile(size_t x, size_t y): _x(x), _y(y), _data(_x, col_type(y)) {
            for(size_t i = 0; i < _data.size(); ++i)
                for(size_t j = 0; j < _data[i].rows.size(); ++j)
                    _data[i][j] = hexpoint_t {i, j};
        }
        size_t x() {return _x;}
        size_t y() {return _y;}
        col_type &operator[](size_t i) {return _data[i];}
        class hexiterator {
        private:
            typedef typename std::vector<col_type>::iterator bigiter;
            typedef typename std::vector<tile_type>::iterator smalliter;
            bigiter _bigiter;
            smalliter _smalliter;
            bigiter _bigend;
            smalliter _smallend;
        public:
            hexiterator(HexTile<T> &cont): _bigiter(cont._data.begin()), _smalliter(_bigiter->rows.begin()), _bigend(cont._data.end()), _smallend(_bigiter->rows.end()) {}
            bool operator==(hexiterator &other) {
                if(_bigend != other._bigend) return false; // wrong iterator pair
                if(other._bigiter == _bigiter && _bigiter == _bigend) return true; // iter == end()?
                return (other._bigiter == _bigiter) && (other._smalliter == _smalliter);
            }
            bool operator!=(hexiterator &other) {return !(*this == other);}
            hexiterator &operator++() {
                ++_smalliter;
                if(_smalliter == _smallend) {
                    ++_bigiter;
                    if(_bigiter == _bigend) return *this;
                    _smalliter = _bigiter->rows.begin();
                    _smallend = _bigiter->rows.end();
                }
                return *this;
            }
            T *operator->() {
                return _smalliter.operator->();
            }
            T &operator*() {
                return *_smalliter;
            }
            hexiterator &end() {
                _bigiter = _bigend;
                return *this;
            }
        };
        hexiterator begin() {return hexiterator(*this);}
        hexiterator end() {return hexiterator(*this).end();}
    };

}

#endif
