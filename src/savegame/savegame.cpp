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
#include "savegame.hpp"

#include<string.h>

using namespace savegame;

SaveGame::SaveGame(std::string p, std::string m, unsigned int v): _path(p), _magic(m), _version(v), _modified(false) {
    generateIndex();
}

SaveGame::~SaveGame() {
    if(_modified) save();
}

bool SaveGame::hasDataBlock(std::string b) {
    for(auto &&i : _blocks) {
        if(i.name == b) return true;
    }

    return false;
}

SaveGame::DataBlock SaveGame::getDataBlock(std::string b) {
    for(auto &&i : _blocks) {
        if(i.name == b) {
            return SaveGame::DataBlock(b, i.size, i.data.get());
        }
    }

    throw datablock_doesnt_exist_error(b);
}

void SaveGame::storeDataBlock(std::string n, uint32_t size, unsigned char *data) {
    for(auto &&i : _blocks) {
        if(i.name == n) {
            i.size = size;
            i.data.reset(data);
            _modified = true;
            return;
        }
    }

    std::unique_ptr<unsigned char> ptr(new unsigned char[size]);
    memcpy(ptr.get(), data, size);
    _blocks.emplace_back(n, size, std::move(ptr));
    _modified = true;
}

void _write(std::ofstream &o, std::string s) {
    uint32_t st = s.size();
    o.write((char *)&st, sizeof(st));
    o.write(s.c_str(), s.size());
}

std::string _read(std::ifstream &i) {
    uint32_t st;
    i.read((char *)&st, sizeof(st));
    std::unique_ptr<char[]> p(new char[st]);
    i.read(p.get(), st);
    return std::string(p.get(), st);
}

void SaveGame::save() {
    std::ofstream out(_path);
    out.exceptions(std::ios::badbit | std::ios::failbit);
    _write(out, _magic);
    out.write((char *)&_version, sizeof(_version));
    uint32_t start = 0;

    for(auto &&i : _blocks) {
        i.start = start;
        _write(out, i.name);
        out.write((char *)&i.size, sizeof(i.size));
        out.write((char *)i.data.get(), i.size);
        i.saved = true;
        start = out.tellp();
    }

    _modified = false;
}

void SaveGame::generateIndex() {
    std::ifstream inp(_path);

    if(inp.fail()) // file doesnt exist
        return;

    inp.exceptions(std::ios::badbit);
    std::string m = _read(inp);

    if(m != _magic) throw magic_error(_magic, m);

    inp.read((char *)&_version, sizeof(_version));

    uint32_t start = 0;

    while(!inp.eof()) {
        std::string name = _read(inp);
        uint32_t size;
        inp.read((char *)&size, sizeof(size));
        std::unique_ptr<unsigned char> ptr(new unsigned char[size]);
        inp.read((char *)ptr.get(), size);
        _blocks.emplace_back(name, start, size, std::move(ptr));
        start += size;
    }
}
