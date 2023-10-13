# print everything
set -x

# if build directory exists, remove it
if [ -d "build" ]; then
  rm -rf build
fi

# create build directory
mkdir build
cd build

# build
cmake -DCMAKE_C_COMPILER=/usr/bin/wasm32-clang -DDANDELION_PLATFORM=wasm ../functions
make