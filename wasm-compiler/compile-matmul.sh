# Convenience script to compile /bin/wasm/matmul.wasm to a sandboxed binary.

# constants
BIN_DIR=bin
SRC=${BIN_DIR}/wasm/matmul.wasm
DST=${BIN_DIR}/matmul

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