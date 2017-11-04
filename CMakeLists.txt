cmake_minimum_required (VERSION 2.6)
project (TeamOr8Bit)
set(CMAKE_CXX_STANDARD 11)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/GLFW)
include_directories(src extern/glad/include extern/glm)
add_executable(TeamOr8Bit src/game/main.cpp extern/glad/src/glad.c)
target_link_libraries(TeamOr8Bit glfw)