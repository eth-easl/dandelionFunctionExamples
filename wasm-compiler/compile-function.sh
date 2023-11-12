# Convenience script to compile a wasm function binary to a sandboxed native binary

# constants
FUNCTION=matmul
BIN_DIR=binaries
SRC=${BIN_DIR}/wasm/${FUNCTION}.wasm
DST=${BIN_DIR}/${FUNCTION}
FUNCTION_SDK_HEAP_SIZE=1         # in pages
FUNCTION_WASM_MIN_HEAP_SIZE=2    # in pages

FUNCTION_WASM_MEM_SIZE=$(($FUNCTION_WASM_MIN_HEAP_SIZE + $FUNCTION_SDK_HEAP_SIZE))
export FUNCTION_SDK_HEAP_SIZE
export FUNCTION_WASM_MEM_SIZE

# exit on error
set -e

# print everathing
set -x

# transpile with rWasm
cd ./rWasm
rm -rf ./generated
cargo run -- --crate-name sandbox-generated --no-alloc --fixed-mem-size ${FUNCTION_WASM_MEM_SIZE} --extern-memory ../${SRC}
cd ..

# compile function
cd ./sandboxed-function
cargo clean
RUSTFLAGS="-C relocation-model=pic" cargo +nightly build --release
# cargo +nightly test -- --nocapture

# copy binary to binaries/
cp ./target/release/libsandboxed_function.so ../${DST}

cd "$(dirname "$0")"