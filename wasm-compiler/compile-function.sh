# Convenience script to compile a wasm function binary to a sandboxed native binary

# constants
FUNCTION=matmul
BIN_DIR=binaries
SRC=${BIN_DIR}/wasm/${FUNCTION}.wasm
DST=${BIN_DIR}/${FUNCTION}
SDK_HEAP_SIZE=1 # in pages
# TODO: find out what the min mem size should be; then add SDK_HEAP_SIZE to it
WASM_MIN_HEAP_SIZE=2 # in pages, for matmul 

# exit on error
set -e

# print everathing
set -x

# transpile with rWasm
cd ./rWasm
rm -rf ./generated
WASM_HEAP_SIZE=$(($WASM_MIN_HEAP_SIZE + $SDK_HEAP_SIZE))
cargo run -- --crate-name sandbox-generated --no-alloc --fixed-mem-size ${WASM_HEAP_SIZE} ../${SRC}
cd ..

# compile function
cd ./sandboxed-function
cargo clean
RUSTFLAGS="-C relocation-model=pic" cargo +nightly build --release

# copy binary to binaries/
cp ./target/release/libsandboxed_function.so ../${DST}

cd "$(dirname "$0")"