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
#ifndef SAVEGAME_HPP
#define SAVEGAME_HPP

#include<type_traits>
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
            std::unique_ptr<unsigned char> data;
            DataBlockInt(std::string n, uint32_t s, std::unique_ptr<unsigned char> &&d): name(n), start(0), size(s), saved(false), data(std::move(d)) {}
            DataBlockInt(std::string n, uint32_t st, uint32_t s, std::unique_ptr<unsigned char> &&d): name(n), start(st), size(s), saved(true), data(std::move(d)) {}
        };
        std::vector<DataBlockInt> _blocks;
        void generateIndex();
    public:
        struct DataBlock {
            std::string name;
            uint32_t size;
            unsigned char *data;
            DataBlock(std::string n, uint32_t s, unsigned char *p): name(n), size(s), data(p) {}
        };
        SaveGame(std::string p, std::string m, unsigned int v);
        SaveGame(SaveGame &&o): _path(std::move(o._path)), _magic(std::move(o._magic)), _version(o._version), _modified(o._modified), _blocks(std::move(o._blocks)) {
            o._modified = false;
        }
        ~SaveGame();
        inline bool operator==(unsigned int version) {
            return _version == version;
        }
        inline bool operator!=(unsigned int version) {
            return _version != version;
        }
        inline bool operator<(unsigned int version) {
            return _version < version;
        }
        inline bool operator>(unsigned int version) {
            return _version > version;
        }
        inline bool operator<=(unsigned int version) {
            return _version <= version;
        }
        inline bool operator>=(unsigned int version) {
            return _version >= version;
        }
        unsigned int version() {
            return _version;
        }
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

    template<unsigned int TYPE, unsigned int VERSION, typename F>
    SaveGame load(std::string p, std::string magic, F &&migrate) {
        SaveGame s(p, magic, VERSION);

        if(s == VERSION) return s; // same version
        else if(s < VERSION) return migrate(std::move(s), s.version(), VERSION); // migrate upwards
        else throw upper_barrier_error();
    }

}

#endif
