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
#include<engine/mesh.hpp>
#include<savegame/savegame.hpp>

#include<glm/vec4.hpp>

#include<config.h>

#ifdef HAS_FREETYPE
#include<ft2build.h>
#include FT_FREETYPE_H
#endif

#include<cmath>

namespace qe {

    /**
     * \brief Loader (data container) for glyphmap data
     */
    template<> class Loader<TEXTG> {
    private:
        size_t _size;
        size_t _length;
        std::unique_ptr<unsigned char[]> _pixels;
    public:
        Loader(savegame::SaveGame::DataBlock &b): _size(b.size), _length(sqrt(b.size)), _pixels(new unsigned char[_size]) {
            memcpy(_pixels.get(), b.data, _size);
        }
        Loader(size_t l): _size(l * l), _length(l), _pixels(new unsigned char[_size]) {}
        unsigned char *parse() {
            return _pixels.get();
        }
        size_t size() {
            return _size;
        }
        size_t width() {
            return _length;
        }
        size_t height() {
            return _length;
        }
        unsigned int elementSize() {
            return 1;
        }
        void setRect(size_t x, size_t y, size_t w, size_t h, unsigned char *data) {
            for(size_t i = 0; i < h; ++i) {
                memcpy(_pixels.get() + (y + h - i) * _length + x, data + i * w, w);
            }
        }
    };

    /**
     * \brief Structure for glyph metrics
     */
    struct fontmetrics {
        size_t x; //!< x coordinate in texture
        size_t y; //!< y coordinate in texture
        size_t h; //!< height in texture
        size_t w; //!< width in texture
        ssize_t off_x; //!< left bearing
        ssize_t off_y; //!< top bearing
        ssize_t adv_x; //!< x advance
        ssize_t adv_y; //!< y advance
    };

    /**
     * \brief Glyphmap for latin (ASCII) characters
     */
    class GlyphmapLatin {
    public:
        static const size_t texlength = 1024; //!< texture width and height
        static const size_t capacity = 256; //!< character count to store in texture
    private:
        std::unique_ptr<Texture<TEXTG, FONTMAPBIND_GL>> _glyphmap; //!< Texture
        std::array<fontmetrics, capacity> _metrics; //!< Metrics
        glm::ivec2 _res; //!< screen resolution
        size_t _linespace; //!< line space (baseline-to-baseline)
        std::string _path; //!< font path
#ifdef HAS_FREETYPE
        /**
         * \brief Initialize glyphmap.
         */
        void initialize(FT_Face face, size_t height);
#endif
    public:
        /**
         * \brief Constructor
         */
#ifdef HAS_FREETYPE
        GlyphmapLatin(std::string bakepath, FT_Face face, size_t height, glm::ivec2 res);
#endif
        GlyphmapLatin(std::string path, glm::ivec2 res);
        GlyphmapLatin(const GlyphmapLatin &other) = delete;
        GlyphmapLatin(GlyphmapLatin &&other) = delete;
        GlyphmapLatin &operator=(const GlyphmapLatin &other) = delete;
        GlyphmapLatin &operator=(GlyphmapLatin &&other) = delete;
        glm::ivec2 getResolution() {
            return _res;
        }
        /**
         * \brief Return pixel height of highest glyph
         */
        size_t highestGlyph() {
            size_t mh = 0;

            for(size_t i = 0; i < capacity; ++i) {
                mh = std::max(mh, _metrics[i].h);
            }

            return mh;
        }
        size_t linespace() {
            return _linespace;
        }
        fontmetrics getMetrics(unsigned char c) {
            return _metrics[c <= 255 ? c : 0];
        }
        /**
         * \brief Return position and scaling vector in screen coordinates
         */
        glm::vec4 scalePosition(glm::ivec2 pos, fontmetrics metrics, float xscale) {
            return (glm::vec4(
                        (1.0f * pos.x + metrics.off_x * xscale),
                        (1.0f * pos.y - (metrics.h - metrics.off_y) * xscale),
                        1.0f * metrics.w * xscale,
                        1.0f * metrics.h * xscale)
                    - glm::vec4(_res.x >> 1, _res.y >> 1, 1, 1))
                   / glm::vec4(_res.x >> 1, _res.y >> 1, _res.x >> 1, _res.y >> 1);
        }
        /**
         * \brief Return UV position of glyph in glyphmap
         */
        glm::vec2 scaleOrigin(fontmetrics m) {
            return glm::vec2(1.0f * m.x / texlength, 1.0f * m.y / texlength);
        }
        /**
         * \brief Return UV scaling of glyph in glyphmap
         */
        glm::vec2 scaleUVScale(fontmetrics m) {
            return glm::vec2(1.0f * m.w / texlength, 1.0f * (1 + m.h) / texlength);
        }
        Texture<TEXTG, FONTMAPBIND_GL> *getTexture() {
            return _glyphmap.get();
        }
        void bake();
    };

    /**
     * \brief Mesh for rendering a glyph
     */
    template<> class Mesh<TEXTG> {
    private:
        Buffer<GL_ARRAY_BUFFER> _buffer; //!< OpenGL buffer
        GLuint _vao; //!< OpenGL VAO handle
        const size_t _size = 4; //!< Vertex count
        const float rectangle[16] = {
            0, 1,
            0, 0,
            1, 1,
            1, 0,
            0, 1,
            0, 0,
            1, 1,
            1, 0
        }; //!< Vertex data
        /**
         * \brief Initialize VAO
         */
        void initVAO() {
            glGenVertexArrays(1, &_vao);
            GLSERRORCHECK;
            glBindVertexArray(_vao);
            GLSERRORCHECK;
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            GLSERRORCHECK;
            _buffer.bind();
            glVertexAttribPointer(
                (GLuint)0, // vertex
                2,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void *)0
            );
            glVertexAttribPointer(
                (GLuint)1, // uv
                2,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void *)32 // 4 * 2 * float
            );
            GLSERRORCHECK;
            glBindVertexArray(0);
        }
    public:
        /**
         * \brief Construct rectangle for text rendering
         */
        Mesh() {
            _buffer.data<GL_DYNAMIC_DRAW>((void *)rectangle, sizeof(rectangle));
            initVAO();
        }
        Mesh(const Mesh<TEXTG> &other) = delete;
        Mesh(Mesh<TEXTG> &&other) = delete;
        /**
         * \brief Destroy VAO and buffer data
         */
        ~Mesh() {
            glDeleteVertexArrays(1, &_vao);
        }
        Mesh<TEXTG> &operator=(const Mesh<TEXTG> &other) = delete;
        Mesh<TEXTG> &operator=(Mesh<TEXTG> &&other) = delete;
        /**
         * \brief Bind VAO and make render call
         */
        void render() {
            glBindVertexArray(_vao);
            GLSERRORCHECK;
            glDrawArrays(GL_TRIANGLE_STRIP, 0, _size);
            GLSERRORCHECK;
        }
    };

}

#endif
