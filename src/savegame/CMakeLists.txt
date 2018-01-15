cmake_minimum_required(VERSION 2.6)
project(savegameOr8Bit)
add_library(libsavegameor8bit_static STATIC
    savegame.cpp
    savegame.hpp)
