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
#ifndef SAVEGAME_HPP
#define SAVEGAME_HPP

#include<fstream>
#include<memory>
#include<stdexcept>
#include<string>
#include<vector>

namespace savegame {

class SaveGame {
private:
    std::string _path;
    std::string _magic;
    uint32_t _version;
    bool _modified;
    struct DataBlockInt {
        std::string name;
        uint32_t start;
        uint32_t size;
        bool saved;
        std::shared_ptr<unsigned char> data;
        DataBlockInt(std::string n, uint32_t s, unsigned char *d): name(n), start(0), size(s), saved(false), data(d) {}
        DataBlockInt(std::string n, uint32_t st, uint32_t s, unsigned char *d): name(n), start(st), size(s), saved(true), data(d) {}
    };
    std::vector<DataBlockInt> _blocks;
    void generateIndex();
public:
    struct DataBlock {
        std::string name;
        uint32_t size;
        std::shared_ptr<unsigned char> data;
        DataBlock(std::string n, uint32_t s, std::shared_ptr<unsigned char> &p): name(n), size(s), data(p) {}
    };
    SaveGame(std::string p, std::string m, unsigned int v);
    ~SaveGame();
    inline bool operator==(unsigned int version) {return _version == version;}
    inline bool operator!=(unsigned int version) {return _version != version;}
    inline bool operator<(unsigned int version) {return _version < version;}
    inline bool operator>(unsigned int version){ return _version > version;}
    inline bool operator<=(unsigned int version){ return _version <= version;}
    inline bool operator>=(unsigned int version){ return _version >= version;}
    bool hasDataBlock(std::string b);
    DataBlock getDataBlock(std::string b);
    void storeDataBlock(std::string n, uint32_t size, unsigned char *data);
    void save();
};

struct upper_barrier_error: public std::runtime_error {
    upper_barrier_error(): runtime_error("SaveGame version too high") {}
};

struct datablock_doesnt_exist_error: public std::runtime_error {
    datablock_doesnt_exist_error(std::string s): runtime_error(std::string("SaveGame.DataBlock does not exist: " + s)) {}
};

struct magic_error: public std::runtime_error {
    magic_error(std::string exp, std::string got): runtime_error(std::string("SaveGame invalid magic value. expected: " + exp + ", got: " + got)) {}
};

template<unsigned int TYPE, unsigned int VERSION> SaveGame &migrate(SaveGame &s) {
    return s;
}

template<unsigned int TYPE, unsigned int VERSION>
SaveGame load(std::string p, std::string magic) {
    SaveGame s(p, magic, VERSION);
    if(s == VERSION) return s; // same version
    else if(s < VERSION) return migrate<TYPE, VERSION - 1>(s); // migrate upwards
    else throw upper_barrier_error();
}

}

#endif
