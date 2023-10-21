This directory contains

- a build script to compile the std-less function examples to wasm
- the compiler to compile .wasm files to provably sandboxed native function binaries

# build script

```bash
./remake-wasm.sh
```

- makes the function examples against the Dandelion SDK targeting `wasm32` in clang, and Dandelion platform `wasm`
- generates .wasm binaries and stores them in `bin/wasm`
- adds .wat files to `bin/wasm` for human-readable versions of the .wasm binaries

# compiler

In `dandelion-wasm-compiler/` run

```bash
cargo run -- <path-to-wasm-file>
```

Specify a `--target` to cross-compile. This will

- compile the .wasm file to a sanboxed Rust crate using rWasm
- compile `sandboxed-function` which is a wrapper around the rWasm-generated crate
- yield `sandboxed-function.so` stored in `bin/<target>` which can be loaded by Dandelion