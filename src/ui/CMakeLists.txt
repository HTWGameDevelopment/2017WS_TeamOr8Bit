cmake_minimum_required(VERSION 2.6)
project(uiOr8Bit)
add_library(libuior8bit_static STATIC
    box.cpp
    box.hpp
    common.hpp
    text.hpp
    ui.hpp)
target_include_directories(libuior8bit_static PUBLIC ..)
