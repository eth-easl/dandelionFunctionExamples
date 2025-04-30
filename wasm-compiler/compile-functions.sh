# Convenience script to compile a wasm function binary to a sandboxed native binary
# To CROSS-compile to aarch64-unknown-linux-gnu, set the ARCH flag to "aarch64"

# Note that the ARCH flag will not affect the wasmtime module pre-compilation. The
# wasmtime modules can only be pre-compiled on the architecture where they will be
# run.

# constants
# the functions to compile
FUNCTIONS="basic matmul matmac busy fan_out_nolibc handle_nolibc template_nolibc"
BIN_DIR=bin
FUNCTION_SDK_HEAP_SIZE=2048     # in 64KB pages, = 128MB
FUNCTION_WASM_MIN_HEAP_SIZE=2   # in pages
DEBUG=0
ARCH="x86_64"                   # "aarch64" or "x86_64"
CLEAN_CARGO=0                   # 0 or 1
DANDELION_TESTS_DIR="../../dandelion/machine_interface/tests/data/"

FUNCTION_WASM_MEM_SIZE=$(($FUNCTION_WASM_MIN_HEAP_SIZE + $FUNCTION_SDK_HEAP_SIZE))
export FUNCTION_SDK_HEAP_SIZE
export FUNCTION_WASM_MEM_SIZE

# exit on error
set -e

if [ $DEBUG -eq 1 ]; then
    # print everything
    set -x
fi

# create necessary bin directories
mkdir -p ${BIN_DIR}/rWasm-sandboxed
mkdir -p ${BIN_DIR}/wasmtime

for FUNC in $FUNCTIONS; do
    echo "\n>>>> COMPILING ${FUNC}\n"
    SRC=${BIN_DIR}/wasm/${FUNC}.wasm

    # compile function with rWasm
    
    RUST_BIN="sandboxed-function/target/${ARCH}-unknown-linux-gnu/release/libsandboxed_function.so"
    DST=${BIN_DIR}/rWasm-sandboxed/${ARCH}-${FUNC}

    cd ./rWasm
    rm -rf ./generated
    cargo run -- --crate-name sandbox-generated --no-alloc --fixed-mem-size ${FUNCTION_WASM_MEM_SIZE} ../${SRC}
    cd ..

    cd ./sandboxed-function
    if [ $CLEAN_CARGO = 1 ]; then 
        cargo clean 
    fi
    cargo +nightly build --release -Z build-std=core --target "${ARCH}-unknown-linux-gnu"
    cd ..
    
    cp ${RUST_BIN} ${DST}

    if [ ! $DANDELION_TESTS_DIR = "" ]; then
        cp ${DST} ${DANDELION_TESTS_DIR}/test_sysld_wasm_${ARCH}_${FUNC}
    fi

    # pre-compile wasmtime module

    DST=${BIN_DIR}/wasmtime/${FUNC}

    cd ./wasmtime-precompile
    cargo run -- ../${SRC} ../${DST}
    cd ..

    # copy binaries to DANDELION_TESTS_DIR

    if [ ! $DANDELION_TESTS_DIR = "" ]; then
        cp ${DST} ${DANDELION_TESTS_DIR}/test_wasmtime_${ARCH}_${FUNC}
    fi

    echo "\n<<<< DONE\n"

done