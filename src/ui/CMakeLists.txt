cmake_minimum_required(VERSION 2.6)
project(uiOr8Bit)
add_library(libuior8bit_static STATIC
    abstractbox.cpp
    abstractbox.hpp
    abstract_common.hpp
    abstracttext.cpp
    abstracttext.hpp
    abstractui.hpp
    CMakeLists.txt
    definedbox.cpp
    definedbox.hpp
    definedtext.hpp
    definedui.cpp
    definedui.hpp
    uifactory.cpp
    uifactory.hpp)
target_include_directories(libuior8bit_static PUBLIC ..)
