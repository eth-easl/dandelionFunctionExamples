 This directory contains

- a build script to compile the std-less function examples to wasm
- the rWasm-based compiler to transpile wasm binaries to Rust
- a wrapper crate which turns the rWasm-generated code into a provably sandboxed native function binary

# wasm binaries build script

```bash
./make-wasm-binaries.sh
```

- makes the function examples against the Dandelion SDK targeting `wasm32` in clang, and Dandelion platform `wasm`
- generates .wasm binaries and stores them in `bin/wasm`
- adds .wat files to `bin/wasm` for human-readable versions of the .wasm binaries

# compile

`compile-matmul.sh` compiles `matmul.wasm` to a native function binary.

## 1. rWasm: `<input>.wasm` -> `sandbox-generated` Rust crate

In `./rWasm` run
    
```bash
cargo run -- --crate-name sandbox-generated --no-alloc --fixed-mem-size <#wasm-mem-pages> ../bin/wasm/<input-file>.wasm
```

## 2. sandboxed-function: `sandbox-generated` Rust crate -> native function binary

In `./sandboxed-function` run

```bash
cargo build --release
```

### cross-compiling to aarch64

Prerequisites:

- add Rust target
    - need to use `nightly` for to cargo's `-Z build-std` feature
    
    ```bash
    rustup +nightly target add aarch64-unknown-linux-gnu
    ```
- install rustup component `rust-src`
    - contains the Rust standard library source code
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

In `./sandboxed-function` now run instead

```bash
cargo +nightly build --release -Z build-std=core --target aarch64-unknown-linux-gnu
```
