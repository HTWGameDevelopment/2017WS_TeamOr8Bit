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
#ifndef QE_LOADER_HPP
#define QE_LOADER_HPP

#include<engine/constants.hpp>

#include<glm/vec3.hpp>

#include<fstream>
#include<vector>
#include<string>

bool startswith(std::string s, std::string f) {
    for(unsigned int i = 0; i < f.size(); ++i)
        if(s[i] != f[i]) return false;
    return true;
}

namespace qe {

template<flag_t t>
class Loader;

struct loader_error: public std::runtime_error {
    loader_error(std::string s, const char* file, int line): runtime_error(std::string(file) + ":" + std::to_string(line) + ": Failed reading " + s) {}
};

template<> class Loader<OBJV1> {
private:
    std::vector<glm::vec3> _vertices;
    std::string _path;
    void load() {
        std::ifstream file(_path);
        file.exceptions(std::ifstream::badbit);
        std::vector<glm::vec3> tvertices;
        std::vector<unsigned int> indices;
        for(std::string line; std::getline(file, line);) {
            if(startswith(line, "v ")) {
                glm::vec3 v;
                switch(sscanf(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z)) {
                case 0:
                case EOF:
                    throw loader_error(std::string(_path) + "(" + line + ")", __FILE__, __LINE__);
                }
                tvertices.push_back(v);
            } else if(startswith(line, "f")) {
                unsigned int vi[3];
                switch(sscanf(line.c_str(), "f %u %u %u", &vi[0], &vi[1], &vi[2])) {
                case 0:
                case EOF:
                    throw loader_error(std::string(_path) + "(" + line + ")", __FILE__, __LINE__);
                }
                indices.push_back(vi[0]);
                indices.push_back(vi[1]);
                indices.push_back(vi[2]);
            }
        }
        if(file.bad() || indices.size() == 0) throw loader_error(_path, __FILE__, __LINE__);
        assert(indices.size() % 3 == 0);
        for(unsigned int i = 0; i < indices.size(); ++i) {
            assert(indices[i] <= tvertices.size());
            _vertices.push_back(tvertices[indices[i] - 1]);
        }
    }
public:
    Loader(std::string path): _path(path) {}
    std::vector<glm::vec3> &parse() {
        if(_vertices.size() != 0) return _vertices;
        load();
        return _vertices;
    }
    unsigned int size() {
        if(_vertices.size() == 0) parse();
        return _vertices.size();
    }
    unsigned int elementSize() {
        return sizeof(glm::vec3);
    }
};

}

#endif
