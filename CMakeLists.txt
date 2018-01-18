cmake_minimum_required (VERSION 2.6)
project (Or8Bit)
# variables
set(TOVERSION_MAJOR 0)
set(TOVERSION_MINOR 0)
set(TEAM_NAME "Team Or8Bit")
set(CMAKE_CXX_STANDARD 14)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_STATIC ON CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_HARDWARE_OPTIMIZATIONS OFF CACHE BOOL "" FORCE)
set(PNG_BUILD_ZLIB "extern/zlib" CACHE PATH "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(DISABLE_FREETYPE OFF CACHE BOOL "Disable freetype rendering and use baked font versions")

set(QE_GLAD_PATH "${CMAKE_SOURCE_DIR}/extern/glad/include" CACHE PATH "" FORCE)
set(QE_GLFW_PATH "${CMAKE_SOURCE_DIR}/extern/GLFW/include" CACHE PATH "" FORCE)
set(QE_PNG_PATH "${CMAKE_SOURCE_DIR}/extern/libpng-1.6.32" CACHE PATH "" FORCE)
set(QE_GLM_PATH "${CMAKE_SOURCE_DIR}/extern/glm" CACHE PATH "" FORCE)

# program
add_executable(Or8Bit
    src/game/main.cpp
    src/game/board.cpp
    src/game/match.cpp
    src/game/unit.cpp
    src/game/screens/game_impl.cpp
    src/game/screens/gameinput.cpp
    src/logger.cpp
    src/font/font_linux.cpp
    src/font/font_win.cpp)

# dependencies
find_package(Freetype)
set(USES_FREETYPE FALSE)
if(Freetype_FOUND AND NOT DISABLE_FREETYPE)
set(USES_FREETYPE TRUE)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(Or8Bit ${FREETYPE_LIBRARIES})
endif()

add_subdirectory(extern/zlib)
add_subdirectory(extern/libpng-1.6.32)
add_subdirectory(extern/GLFW)
add_subdirectory(src/engine)
add_subdirectory(src/ui)
add_subdirectory(src/savegame)

# config header
configure_file(src/config.h.in config.h)

# link program
target_link_libraries(Or8Bit png_static zlibstatic glfw libquarterengine_static libuior8bit_static libsavegameor8bit_static)
