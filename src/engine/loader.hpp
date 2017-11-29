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
#include<glm/vec2.hpp>

#include<png.h>

#include<array>
#include<fstream>
#include<memory>
#include<vector>
#include<string>

extern "C" {
    unsigned char *_qe_read_png(FILE *fd, size_t *size, size_t *width, size_t *height);
    enum _qe_read_png_error_t {READ_STRUCT = 1, INFO_STRUCT = 2, READ_IMAGE = 3};
    extern enum _qe_read_png_error_t _qe_read_png_error;
}

static bool startswith(std::string s, std::string f) {
    for(unsigned int i = 0; i < f.size(); ++i)
        if(s[i] != f[i]) return false;

    return true;
}

namespace qe {

    template<flag_t t>
    class Loader;

    /**
     * \brief Exception from Loader
     */
    struct loader_error: public std::runtime_error {
        /**
         * \brief Constructs a new loader_error
         *
         * \param s Message
         * \param file File path
         * \param line Source line
         */
        loader_error(std::string s, const char *file, int line): runtime_error(std::string(file) + ":" + std::to_string(line) + ": Failed reading " + s) {}
    };

    /**
     * \brief Data about one vertex in OBJV2 meshes
     */
    struct objv2_point_t {
        glm::vec3 vertex; //!< vertex location
        glm::vec2 uv; //!< UV coordinate
        glm::vec3 normal; //!< Normal
        /**
         * \brief Constructor
         */
        objv2_point_t(glm::vec3 v, glm::vec2 u, glm::vec3 n): vertex(v), uv(u), normal(n) {}
    };

    /**
     * \brief Loader for RGBA png files
     */
    template<> class Loader<PNGRGBA> {
    private:
        std::unique_ptr<unsigned char[]> _pixels; //!< Pixel array
        size_t _size; //!< Size of pixel array in bytes
        size_t _width; //!< Width in pixels
        size_t _height; //!< Height in pixels
        std::string _path; //!< Origin path
        /**
         * \brief Load data from file
         */
        void load() {
            // Open file
            std::unique_ptr<std::FILE, decltype(&std::fclose)> fd(fopen(_path.c_str(), "r"), &std::fclose);

            if(!fd) throw loader_error(std::string(_path), __FILE__, __LINE__);

            unsigned char header[8];
            std::fread(header, 1, 8, fd.get());

            if(png_sig_cmp(header, 0, 8)) throw loader_error(std::string(_path) + " (png_sig_cmp)", __FILE__, __LINE__);

            _pixels.reset(_qe_read_png(fd.get(), &_size, &_width, &_height));

            if(!_pixels.get()) {
                switch(_qe_read_png_error) {
                case READ_STRUCT:
                    throw loader_error(std::string(_path) + " (png: read_struct)", __FILE__, __LINE__);

                case INFO_STRUCT:
                    throw loader_error(std::string(_path) + " (png: read_struct)", __FILE__, __LINE__);

                default:
                    throw loader_error(std::string(_path) + " (png: read_struct)", __FILE__, __LINE__);
                }
            }

            if(_size == 0) throw loader_error(std::string(_path) + " (read_png)", __FILE__, __LINE__);
        }
    public:
        /**
         * \brief Construct loader from file path
         */
        Loader(std::string path): _path(path), _size(0) {}
        /**
         * \brief Parse file and return data pointer
         */
        unsigned char *parse() {
            if(_size != 0) return _pixels.get();

            load();
            return _pixels.get();
        }
        /**
         * \brief Return size of pixel array
         */
        size_t size() {
            if(_size == 0) load();

            return _size;
        }
        /**
         * \brief Return width of pixel array
         */
        size_t width() {
            if(_size == 0) load();

            return _width;
        }
        /**
         * \brief Return height of pixel array
         */
        size_t height() {
            if(_size == 0) load();

            return _height;
        }
        /**
         * \brief Return element size of pixel in bytes
         */
        unsigned int elementSize() {
            return sizeof(unsigned char[4]);
        }
    };

    /**
     * \brief Loader for .objv2 meshes
     */
    template<> class Loader<OBJV2> {
    private:
        std::vector<objv2_point_t> _points;
        std::string _path;
        /**
         * \brief Load data from file
         */
        void load() {
            std::ifstream file(_path);
            file.exceptions(std::ifstream::badbit);
            std::vector<glm::vec3> tvertices;
            std::vector<glm::vec2> tuvs;
            std::vector<glm::vec3> tnormals;
            std::vector<glm::ivec3> indices;

            for(std::string line; std::getline(file, line);) {
                if(startswith(line, "v ")) {
                    glm::vec3 v;

                    switch(sscanf(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z)) {
                    case 0:
                    case EOF:
                        throw loader_error(std::string(_path) + "(" + line + ")", __FILE__, __LINE__);
                    }

                    tvertices.push_back(v);
                } else if(startswith(line, "vt ")) {
                    glm::vec2 v;

                    switch(sscanf(line.c_str(), "vt %f %f", &v.x, &v.y)) {
                    case 0:
                    case EOF:
                        throw loader_error(std::string(_path) + "(" + line + ")", __FILE__, __LINE__);
                    }

                    v.y = 1 - v.y; // Invert UV.y coordinate
                    tuvs.push_back(v);
                } else if(startswith(line, "vn ")) {
                    glm::vec3 v;

                    switch(sscanf(line.c_str(), "vn %f %f %f", &v.x, &v.y, &v.z)) {
                    case 0:
                    case EOF:
                        throw loader_error(std::string(_path) + "(" + line + ")", __FILE__, __LINE__);
                    }

                    tnormals.push_back(v);
                } else if(startswith(line, "f ")) {
                    std::array<glm::ivec3, 3> points;

                    switch(sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
                                  &points[0].x, &points[0].y, &points[0].z,
                                  &points[1].x, &points[1].y, &points[1].z,
                                  &points[2].x, &points[2].y, &points[2].z)) {
                    case 0:
                    case EOF:
                        throw loader_error(std::string(_path) + " (" + line + ")", __FILE__, __LINE__);
                    }

                    indices.push_back(points[0]);
                    indices.push_back(points[1]);
                    indices.push_back(points[2]);
                }
            }

            if(file.bad()) throw loader_error(_path, __FILE__, __LINE__);

            if(indices.size() == 0) throw loader_error(std::string(_path) + " (no indices found)", __FILE__, __LINE__);

            assert(indices.size() % 3 == 0);

            for(unsigned int i = 0; i < indices.size(); ++i) {
                assert(indices[i].x <= tvertices.size());
                assert(indices[i].y <= tuvs.size());
                assert(indices[i].z <= tnormals.size());
                glm::ivec3 index = indices[i];
                _points.emplace_back(tvertices[index.x - 1], tuvs[index.y - 1], tnormals[index.z - 1]);
            }
        }
    public:
        /**
         * \brief Construct loader from file path
         */
        Loader(std::string path): _path(path) {}
        /**
         * \brief Parse file and return data vector
         */
        std::vector<objv2_point_t> &parse() {
            if(_points.size() != 0) return _points;

            load();
            return _points;
        }
        /**
         * \brief Return size of pixel array
         */
        unsigned int size() {
            if(_points.size() == 0) load();

            return _points.size();
        }
        /**
         * \brief Return element size of vertex in bytes
         */
        unsigned int elementSize() {
            return sizeof(objv2_point_t);
        }
    };

    /**
     * \brief Loader for .objv1 meshes
     */
    template<> class Loader<OBJV1> {
    private:
        std::vector<glm::vec3> _vertices;
        std::string _path;
        /**
         * \brief Load data from file
         */
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
        /**
         * \brief Construct loader from file path
         */
        Loader(std::string path): _path(path) {}
        /**
         * \brief Parse file and return data vector
         */
        std::vector<glm::vec3> &parse() {
            if(_vertices.size() != 0) return _vertices;

            load();
            return _vertices;
        }
        /**
         * \brief Return size of pixel array
         */
        unsigned int size() {
            if(_vertices.size() == 0) parse();

            return _vertices.size();
        }
        /**
         * \brief Return element size of vertex in bytes
         */
        unsigned int elementSize() {
            return sizeof(glm::vec3);
        }
    };

}

#endif
