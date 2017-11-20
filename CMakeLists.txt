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
# compile external dependencies
add_subdirectory(extern/zlib)
add_subdirectory(extern/libpng-1.6.32)
add_subdirectory(extern/GLFW)
add_definitions(${PNG_DEFINITIONS})
# generate config header
configure_file(src/config.h.in config.h)
# compile program
include_directories(${CMAKE_BINARY_DIR} src extern/glad/include extern/glm extern/GLFW/include extern/libpng-1.6.32)
add_executable(TeamOr8Bit src/game/main.cpp extern/glad/src/glad.c src/engine/loader.c src/engine/screenshot.cpp src/logger.cpp)
# link program
target_link_libraries(TeamOr8Bit png_static zlibstatic glfw)
