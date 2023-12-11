use wasmtime;
use std::env;
use std::path::Path;

// const TEST_DESERIALIZE: bool = true;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 3 {
        eprintln!("Usage: {} <path_to_wasm_binary> <destination_path>", args[0]);
        std::process::exit(1);
    }

    let wasm_path = &args[1];
    let dest_path = &args[2];
    println!("Serializing .wasm binary for wasmtime...");

    let engine = wasmtime::Engine::default();
    let mut _store = wasmtime::Store::new(&engine, ());

    let p = Path::new(wasm_path);
    println!("Processing {:?}", p);

    let module = wasmtime::Module::from_file(&engine, &p).unwrap();
    let serialized = module.serialize().unwrap();

    // // test module instantiation
    // if TEST_DESERIALIZE {
    //     let m = unsafe { wasmtime::Module::deserialize(&engine, &serialized).unwrap() };
    //     let instance = wasmtime::Instance::new(&mut store, &m, &[]).unwrap();
    //     let entry_ = instance.get_typed_func::<(), ()>(&mut store, "_start").unwrap();
    // }

    // store the serialized module in the specified destination path
    let path = Path::new(dest_path).to_path_buf();
    std::fs::write(path, serialized).unwrap();

    println!("done");
}