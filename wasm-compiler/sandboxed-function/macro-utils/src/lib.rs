extern crate sandbox_generated;
use sandbox_generated as Generated;

//
// Helper macros to extract data necessary for Dandelion from the generated
// code at compile time.
//

macro_rules! generate_getter {
    ($name:ident) => {
        #[proc_macro]
        #[allow(non_snake_case)]
        pub fn $name(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
            #[allow(unused_mut)]
            let mut module = Generated::WasmModule::new();
            let $name = module.$name().unwrap();
            let $name = format!("0x{:x}", $name);
            $name.parse().unwrap()
        }
    }
}

generate_getter!(__wasm_sysdata_region_size);
generate_getter!(__wasm_sysdata_region_base);
generate_getter!(__wasm_sdk_heap_size); // in pages!
generate_getter!(get___heap_base);
generate_getter!(get___dandelion_system_data);

#[proc_macro]
/// The wasm module's memory size in bytes.
pub fn get_memory_size(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
    let module = Generated::WasmModule::new();
    let mem_size = module.get_memory_size();
    let mem_size = format!("0x{:x}", mem_size);
    mem_size.parse().unwrap()
}