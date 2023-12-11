 This directory contains

- a UNIX shell script compiling the std-less function examples to `.wasm` binaries in `bin/wasm/` using
    - clang and the cmake files
- a UNIX shell script compiling the `.wasm` binaries to native binaries in `bin/` and precompiling wasmtime modules, using
    - the rWasm-based compiler to transpile WASM binaries to Rust
        - this will generate a Rust crate in `rWasm/generated`
    - a wrapper crate which turns `rWasm/generated` into a provably sandboxed native binary
    - a Rust crate `wasmtime-precompile` pre-compiling the `.wasm` binaries for wasmtime in `bin/wasmtime/`

# wasm binaries build script

```bash
./make-wasm-binaries.sh
```

- makes the function examples against the Dandelion SDK targeting `wasm32` in clang, and Dandelion platform `wasm`
- generates .wasm binaries and stores them in `binaries/wasm`
- adds .wat files to `bin/wasm` for human-readable versions of the .wasm binaries

# compile

`compile-function.sh` compiles the functions listed in the script to native binaries, invoking the rWasm compiler and then compiling a wrapper crate for each.

**Note**: By setting `AARCH64_CROSS_COMPILE` in the script it will attempt to **cross**-compile to `aarch64-unknown-linux-gnu`. When cross-compiling to another target, make sure to fix the `compiler_builtins` dependency in `sandboxed-function`.

aarch64 cross-compilation prerequisites:

- add Rust target
    - need to use `nightly` for cargo's `-Z build-std` feature
    
    ```bash
    rustup +nightly target add aarch64-unknown-linux-gnu
    ```
- install rustup component `rust-src`
    - contains the Rust standard (including core) library source code
    - required by cargo's `-Z build-std` feature to cross-compile `core` in our case
    
    ```bash
    rustup component add rust-src --toolchain nightly
    ```
- install the corresponding GCC (used for cross-linking)
    
    ```bash
    sudo apt-get install gcc-aarch64-linux-gnu
    ```
- add `./sandboxed-function/.cargo/config.toml`
    
    ```bash
    [target.aarch64-unknown-linux-gnu]
    linker = "aarch64-linux-gnu-gcc"
    ```