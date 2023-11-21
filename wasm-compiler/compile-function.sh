# Convenience script to compile a wasm function binary to a sandboxed native binary
# To CROSS-compile to aarch64-unknown-linux-gnu, set the AARCH64_CROSS_COMPILE flag to 1
# If not set, it will simply compile to the host architecture

# constants
# the functions to compile
FUNCTIONS="basic matmul matmac"
BIN_DIR=binaries
FUNCTION_SDK_HEAP_SIZE=2048     # in 64KB pages, = 128MB
FUNCTION_WASM_MIN_HEAP_SIZE=2   # in pages
DEBUG=0
AARCH64_CROSS_COMPILE=1         # 0 (host) or "aarch64-unknown-linux-gnu"
CLEAN_CARGO=0                   # 0 or 1

FUNCTION_WASM_MEM_SIZE=$(($FUNCTION_WASM_MIN_HEAP_SIZE + $FUNCTION_SDK_HEAP_SIZE))
export FUNCTION_SDK_HEAP_SIZE
export FUNCTION_WASM_MEM_SIZE

# exit on error (doesn't do what I expected)
set -e

if [ $DEBUG -eq 1 ]; then
    # print everything
    set -x
fi

for FUNC in $FUNCTIONS; do
    echo "\n>>>> COMPILING ${FUNC}\n"
    
    SRC=${BIN_DIR}/wasm/${FUNC}.wasm
    RUST_BIN=$( 
        if [ $AARCH64_CROSS_COMPILE = 1 ]; 
        then    echo "sandboxed-function/target/aarch64-unknown-linux-gnu/release/libsandboxed_function.so";
        else    echo "sandboxed-function/target/release/libsandboxed_function.so";
        fi 
    )
    DST=${BIN_DIR}/${FUNC}$(
        if [ $AARCH64_CROSS_COMPILE = 1 ]; then echo "-aarch64"; fi
    )

    # transpile with rWasm
    cd ./rWasm
    rm -rf ./generated
    cargo run -- --crate-name sandbox-generated --no-alloc --fixed-mem-size ${FUNCTION_WASM_MEM_SIZE} --extern-memory ../${SRC}
    cd ..

    # compile function wrapper
    cd ./sandboxed-function
    if [ $CLEAN_CARGO = 1 ]; then
        cargo clean
    fi
    if [ $AARCH64_CROSS_COMPILE = 0 ]; 
    then    cargo +nightly build --release
    else    cargo +nightly build --release -Z build-std=core --target "aarch64-unknown-linux-gnu"
    fi
    cd ..

    # copy binary to binaries/
    cp ${RUST_BIN} ${DST}

    echo "\n<<<< DONE\n"

done