 This directory contains

- a build script to compile the std-less function examples to wasm
- the rWasm-based compiler to transpile wasm binaries to Rust
- a wrapper crate which turns the rWasm-generated code into a provably sandboxed native function binary

# wasm binaries build script

```bash
./remake-wasm.sh
```

- makes the function examples against the Dandelion SDK targeting `wasm32` in clang, and Dandelion platform `wasm`
- generates .wasm binaries and stores them in `bin/wasm`
- adds .wat files to `bin/wasm` for human-readable versions of the .wasm binaries

# compile

See `compile-matmul.sh`, it compiles `matmul.wasm` to a native function binary.