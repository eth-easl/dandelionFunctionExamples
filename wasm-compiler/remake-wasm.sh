# A convenience script to re-compile the example functions to wasm.
# Script should be run from the directory it is in.
# It will copy the wasm binaries to ./wasm-binaries.

# set constants
export WASM_CLANG=/usr/bin/wasm32-clang
# export WASM_OPT=~/projects/wasm/binaryen/bin/wasm-opt
# export WASM_OPT_FLAGS="-O3"

# print everything
set -x

# build wasm functions

cd "$(dirname "$0")/.."

if [ -d "build" ]; then
  rm -rf build
fi
mkdir build
cd build

cmake -DCMAKE_C_COMPILER="$WASM_CLANG" -DDANDELION_PLATFORM=wasm ../functions
make

cd ..

# copy wasm binaries to wasm-compiler/wasm-binaries

cd wasm-compiler

if [ -d "wasm-binaries" ]; then
  rm -rf ./wasm-binaries
fi

mkdir wasm-binaries

cd ../build/

for d in ./*; do
  if [ -d "$d" ]; then
    if [ "$d" != "./CMakeFiles" ] && [ "$d" != "./functionInterface" ]; then
      cp "$d/$d" "../wasm-compiler/wasm-binaries/$d.wasm"
    fi
  fi
done

cd ../wasm-compiler/wasm-binaries

# for each wasm binary here, create a .wat file in the same location

for d in ./*.wasm; do
  wasm2wat "$d" -o "${d%.wasm}.wat"
done

# move back to the directory of this script
cd "$(dirname "$0")"
