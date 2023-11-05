# Convenience script to compile a wasm function binary to a sandboxed native binary

# constants
FUNCTION=matmul
BIN_DIR=bin
SRC=${BIN_DIR}/wasm/${FUNCTION}.wasm
DST=${BIN_DIR}/${FUNCTION}

# exit on error
set -e

# print everathing
set -x

# transpile with rWasm
cd ./rWasm
rm -rf ./generated
cargo run -- --crate-name sandbox-generated --no-alloc --fixed-mem-size 2 ../${SRC}

# compile function
cd ../sandboxed-function
cargo clean
RUSTFLAGS="-C relocation-model=pic" cargo +nightly build --release

# copy binary to bin/
cp ./target/release/libsandboxed_function.so ../${DST}

cd "$(dirname "$0")"