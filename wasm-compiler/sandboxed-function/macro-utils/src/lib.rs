extern crate sandbox_generated;
use std::str::FromStr;

use sandbox_generated as Generated;

//
// Helper macros to extract data necessary for Dandelion from the generated
// code at compile time.
//

#[proc_macro]
pub fn get_system_data_wasm_offset(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
    let module = Generated::WasmModule::new();
    let system_data = module.get___dandelion_system_data().unwrap();
    let system_data = format!("0x{:x}", system_data);
    proc_macro::TokenStream::from_str(&system_data).unwrap()
}