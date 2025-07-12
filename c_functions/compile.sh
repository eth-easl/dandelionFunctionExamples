rm -rf build
mkdir build
cd build
cmake -DPLATFORM=MMU_LINUX -DARCHITECTURE=x86_64 -DCMAKE_BUILD_TYPE=Debug ..
make -j