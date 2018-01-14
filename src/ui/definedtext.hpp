// Copyright (c) 2018 Fabian Stiewitz
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
#ifndef UI_DEFINED_TEXT_HPP
#define UI_DEFINED_TEXT_HPP

#include<ui/abstract_common.hpp>

#include<string>

inline std::string replaceNewline(std::string t) {
    std::string r = t;
    size_t i = 0;
    while((i = r.find('\n', i)) != std::string::npos)
        r.replace(i, 1, "\\n");
    return r;
}

namespace ui {

    class DefinedText: public DefinedRenderable {
    private:
        std::string _text;
    public:
        std::string &text() {
            return _text;
        }
        virtual void __introspect(size_t off) {
            DefinedRenderable::__introspect(off);
            std::cout << std::string(off + 2, ' ') << "Text = '" << replaceNewline(_text) << "'" << std::endl;
        }
    };

}

#endif
