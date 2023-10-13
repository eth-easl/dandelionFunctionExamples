# set constants
export WASM_CLANG=/usr/bin/wasm32-clang
export WASM_OPT=~/projects/wasm/binaryen/bin/wasm-opt
export WASM_OPT_FLAGS="-O3"

# print everything
set -x

# re-create build directory

if [ -d "build" ]; then
  rm -rf build
fi
mkdir build
cd build

# build

cmake -DCMAKE_C_COMPILER="$WASM_CLANG" -DDANDELION_PLATFORM=wasm ../functions
make

# iterate through subdirectories build/<subdir> and rename build/<subdir>/<subdir> to build/<subdir>/<subdir>.wasm
# exceptions: CMakeFiles, functionInterface

for d in ./*; do
  if [ -d "$d" ]; then
    if [ "$d" != "./CMakeFiles" ] && [ "$d" != "./functionInterface" ]; then
      mv "$d/$d" "$d/$d.wasm"
    fi
  fi
done

# run wasm-opt on each .wasm file

for d in ./*; do
  if [ -f "$d/$d.wasm" ]; then
    "$WASM_OPT" "$d/$d.wasm" "$WASM_OPT_FLAGS" -o "$d/$d.wasm"
  fi
done

# for each wasm binary, create a .wat file in the same location

for d in ./*; do
  if [ -f "$d/$d.wasm" ]; then
    wasm2wat "$d/$d.wasm"  -o "$d/$d.wat"
  fi
done
