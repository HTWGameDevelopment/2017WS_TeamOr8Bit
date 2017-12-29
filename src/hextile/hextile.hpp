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

#include<functional>
#include<memory>
#include<string>
#include<vector>

namespace hextile {

    struct hexpoint_t {
        int x;
        int y;
        std::string string() {
            return std::string("(") + std::to_string(x) + "," + std::to_string(y) + ")";
        }
    };

    struct marker_t {
        unsigned int id;
        unsigned int val;
    };

    /**
     * \brief A 2D board of hexagonal tiles of type T
     */
    template<typename T, unsigned int L>
    class HexTile {
    public:
        typedef T tile_type;
        /**
         * \brief Sub-class representing one row of tiles
         */
        struct col_type {
            std::vector<tile_type> rows;
            col_type() {}
            col_type(const col_type &other) = delete;
            col_type(col_type &&other): rows(std::move(other.rows)) {}
            col_type &operator=(const col_type &other) = delete;
            col_type &operator=(col_type &&other) {
                rows = std::move(other.rows);
                return *this;
            }
            tile_type &operator[](size_t i) {
                return rows[i];
            }
            void push_back(tile_type &&t) {
                rows.push_back(std::move(t));
            }
        };
    protected:
        size_t _x;
        size_t _y;
        std::vector<col_type> _data;
        std::array<unsigned int, L> _marker_ids;
        unsigned int _current_layer;
        class edgeiterator {
        private:
            HexTile<T, L> *_board;
            hexpoint_t _coord;
            unsigned int _step;
        public:
            edgeiterator(HexTile<T, L> *board, hexpoint_t coord): _board(board), _coord(coord), _step(0) {}
            T *next() {
                auto step = _step++;

                switch(step) {
                case 0:
                    if(inbounds(_coord.x, _coord.y - 1)) return &(*_board)[_coord.x][_coord.y - 1];

                case 1:
                    if(inbounds(_coord.x, _coord.y + 1)) return &(*_board)[_coord.x][_coord.y + 1];

                case 2:
                    if(inbounds(_coord.x - 1, _coord.y)) return &(*_board)[_coord.x - 1][_coord.y];

                case 3:
                    if(inbounds(_coord.x + 1, _coord.y)) return &(*_board)[_coord.x + 1][_coord.y];

                case 4:
                    if(inbounds(_coord.x + 1 - ((_coord.y % 2) * 2), _coord.y + 1)) return &(*_board)[_coord.x + 1 - ((_coord.y % 2) * 2)][_coord.y + 1];

                case 5:
                    if(inbounds(_coord.x + 1 - ((_coord.y % 2) * 2), _coord.y - 1)) return &(*_board)[_coord.x + 1 - ((_coord.y % 2) * 2)][_coord.y - 1];

                default:
                    return nullptr;
                }
            }
            bool inbounds(ssize_t x, ssize_t y) { // unsigned to signed. just use signed? TODO
                return (x >= 0 && x < _board->x() - 1)
                    && (y >= 0 && y < _board->y() - 1);
            }
        };
        template<typename P>
        void markByEdgeIterator(edgeiterator iter, P payload, std::function<bool(T&,P&)> relation) {
            P save = payload;
            while(T *n = iter.next()) {
                if(relation(*n, payload)) {
                    markByEdgeIterator(edgeiterator(this, n->coord()), payload, relation);
                }
                payload = save;
            }
        }
    public:
        HexTile(size_t x, size_t y): _x(x), _y(y) {
            for(int i = 0; i < L; ++i)
                _marker_ids[i] = 0;
            for(int i = 0; i < _x; ++i) {
                col_type t;
                for(int j = 0; j < _y; ++j) {
                    t.push_back(T(nullptr, hexpoint_t {i, j}));
                }
                _data.push_back(std::move(t));
            }
        }
        HexTile(const HexTile &other) = delete;
        HexTile(HexTile &&other): _x(other._x), _y(other._y), _data(std::move(other._data)), _marker_ids(other._marker_ids) {}
        ssize_t x() {
            return _x;
        }
        ssize_t y() {
            return _y;
        }
        col_type &operator[](size_t i) {
            return _data[i];
        }
        T &get(hexpoint_t i) {
            return _data[i.x][i.y];
        }
        /**
         * \brief Iterator over all tiles
         */
        class hexiterator {
        private:
            typedef typename std::vector<col_type>::iterator bigiter;
            typedef typename std::vector<tile_type>::iterator smalliter;
            bigiter _bigiter;
            smalliter _smalliter;
            bigiter _bigend;
            smalliter _smallend;
        public:
            hexiterator(HexTile<T, L> &cont): _bigiter(cont._data.begin()), _smalliter(_bigiter->rows.begin()), _bigend(cont._data.end()), _smallend(_bigiter->rows.end()) {}
            bool operator==(hexiterator &other) {
                if(_bigend != other._bigend) return false; // wrong iterator pair

                if(other._bigiter == _bigiter && _bigiter == _bigend) return true; // iter == end()?

                return (other._bigiter == _bigiter) && (other._smalliter == _smalliter);
            }
            bool operator!=(hexiterator &other) {
                return !(*this == other);
            }
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
        hexiterator begin() {
            return hexiterator(*this);
        }
        hexiterator end() {
            return hexiterator(*this).end();
        }
        unsigned int currentMarker(unsigned int layer) {
            return _marker_ids[layer];
        }
        template<typename P>
        void markByEdge(hexpoint_t start, P init, unsigned int layer, std::function<bool(T&,P&)> relation) {
            ++_marker_ids[layer];
            _current_layer = layer;
            markByEdgeIterator(edgeiterator(this, start), init, relation);
        }
        void clearMarker(unsigned int layer) {
            ++_marker_ids[layer];
        }
    };

}

#endif
