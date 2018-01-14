cmake_minimum_required(VERSION 2.6)
project(quarterengine)

set(QE_GLAD_PATH "glad/include" CACHE PATH "Path to glad headers")
set(QE_GLFW_PATH "GLFW/include" CACHE PATH "Path to GLFW headers")
set(QE_PNG_PATH "libpng/include" CACHE PATH "Path to libpng headers")
set(QE_GLM_PATH "glm" CACHE PATH "Path to glm headers")

add_library(libquarterengine_static STATIC
    buffer.hpp
    cache.cpp
    cache.hpp
    camerabounds.hpp
    camera.hpp
    constants.hpp
    context.hpp
    glyphmap.cpp
    glyphmap.hpp
    loader.c
    loader.hpp
    mesh.cpp
    mesh.hpp
    paths.hpp
    programs.cpp
    programs.hpp
    qe.hpp
    screenshot.cpp
    screenshot.hpp
    text.hpp
    textures.hpp
    ${QE_GLAD_PATH}/../src/glad.c)
target_include_directories(libquarterengine_static PUBLIC .. ${CMAKE_BINARY_DIR} ${QE_GLAD_PATH} ${QE_GLM_PATH} ${QE_GLFW_PATH} ${QE_PNG_PATH})
