git submodule init
git submodule update
cd extern/glew
git checkout glew-2.1.0
make extensions
make
cd ../GLFW
git checkout 3.2.1
cmake .
make
cd ../glm
cmake .
make
cd ../../src
tup init
cd ..
mkdir -p docs src/{engine,game} assets/{models,textures,sound,anim,shaders} out
