git submodule init
git submodule update
cd extern/GLFW
git checkout 3.2.1
cd ..
cp zlib-CMakeLists.txt zlib/
cd ..
mkdir -p docs src/{engine,game} build assets/{models,textures,sound,anim,shaders} out
cd build
cmake ..
