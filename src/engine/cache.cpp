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
#include "cache.hpp"

using namespace qe;

GlyphmapLatin *Cache::glyphlatin;
qe::Program *Cache::objv1;
qe::Program *Cache::objv2;
qe::Program *Cache::objv3;
qe::Program *Cache::texts;
qe::Program *Cache::sprite2d;
qe::Program *Cache::sprite2dcolor;
qe::Mesh<TEXTG> *Cache::meshm;

void Cache::__introspect(size_t off) {
    std::cout << std::string(off, ' ') << "Cache" << std::endl;
    glyphlatin->__introspect(off + 2);
    objv1->__introspect(off + 2);
    objv2->__introspect(off + 2);
    objv3->__introspect(off + 2);
    texts->__introspect(off + 2);
    sprite2d->__introspect(off + 2);
    sprite2dcolor->__introspect(off + 2);
    meshm->__introspect(off + 2);
}
