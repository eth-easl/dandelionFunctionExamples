# A convenience script to re-compile the example functions to wasm.
# Script should be run from the directory it is in.
# It will copy the wasm binaries to bin/wasm/.
# Requires /usr/bin/wasm32-clang to be installed. If your wasm32-clang is
# installed somewhere else, change the WASM_CLANG variable below.

# clang configuration for wasm
#export WASM_CLANG=/usr/bin/clang-17

BIN_DIR=bin
DANDELION_TESTS_DIR="../../dandelion/machine_interface/tests/data"

# (currently unused) wasm-opt configuration, a wasm-to-wasm optimizer from binaryen
# export WASM_OPT=~/projects/wasm/binaryen/bin/wasm-opt
# export WASM_OPT_FLAGS="-O3"

# print everything
set -x

# create necessary bin directories
mkdir -p ${BIN_DIR}/wasm

# build wasm functions

cd "$(dirname "$0")/.."

if [ -d "build" ]; then
  rm -rf build
fi
mkdir build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE="./dandelion.cmake" -DCMAKE_C_COMPILER="$WASM_CLANG" -DDANDELION_PLATFORM=wasm ../functions
make

# copy wasm binaries to wasm-compiler/bin/wasm
mkdir -p ../wasm-compiler/bin/wasm
for d in ./*; do
  if [ -d "$d" ]; then
    if [ "$d" == "./example_app_nolibc" ]; then
      echo "TEST PRINT"
      cp "$d/template_nolibc" "../wasm-compiler/bin/wasm/template_nolibc.wasm"
      cp "$d/handle_nolibc" "../wasm-compiler/bin/wasm/handle_nolibc.wasm"
      cp "$d/fan_out_nolibc" "../wasm-compiler/bin/wasm/fan_out_nolibc.wasm"
    elif [ "$d" != "./CMakeFiles" ] && [ "$d" != "./functionInterface" ]; then
      cp "$d/$d" "../wasm-compiler/bin/wasm/$d.wasm"
    fi
  fi
done

cd ../wasm-compiler/bin/wasm

# for each wasm binary here, create a .wat file in the same location
for d in ./*.wasm; do
  wasm2wat "$d" -o "${d%.wasm}.wat"
done

# copy wasm binaries to Dandelion (without the .wasm extension)
if [ ! $DANDELION_TESTS_DIR = "" ]; then
  for d in ./*.wasm; do
    filename=$(basename -- "$d")
    cp "$d" "../../${DANDELION_TESTS_DIR}/test_wasm_${filename%.wasm}"
  done
fi
