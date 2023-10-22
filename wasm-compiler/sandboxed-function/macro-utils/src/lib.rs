extern crate sandbox_generated;
use sandbox_generated as Generated;

//
// Helper macros to extract data necessary for Dandelion from the generated
// code at compile time.
//

#[proc_macro]
pub fn get_system_data_wasm_offset(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
    println!("get_system_data_wasm_offset");
    let module = Generated::WasmModule::new();
    let system_data = module.get___dandelion_system_data().unwrap();
    let system_data_str = format!("0x{:x}", system_data);
    system_data_str.parse().unwrap()
}