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
#ifndef QE_CONSTANTS_HPP
#define QE_CONSTANTS_HPP

#include<type_traits>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace qe {
typedef unsigned int flag_t;

const char* backend_error_glfw = "could not initialize glfw";
const char* backend_error_window = "could not create window";
const char* backend_error_glew = "could not initialize glew";

const flag_t VERTEX = 0;
const flag_t FRAGMENT = 1;
const flag_t OBJV1 = 2;
const flag_t OBJV2 = 3;
const flag_t PNGRGBA = 4;

// UNIFORMS
const flag_t UNIMVP = 0;
const flag_t UNIM = 1;
const flag_t UNIV = 2;
const flag_t UNIL = 3;
const flag_t UNIDIFFTEX = 4;

// TEXTURE BINDING POINTS
const flag_t DIFFTEXBIND = 0;

}

#endif
