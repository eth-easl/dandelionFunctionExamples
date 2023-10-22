// #![no_std]

extern crate sandbox_generated;
use std::str::FromStr;

use sandbox_generated as Generated;

// extern crate proc_macro;
// use proc_macro2::TokenStream;

//
// Helper macros to extract data necessary for Dandelion from the generated
// code at compile time.
//

#[proc_macro]
pub fn get_system_data(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
    let module = Generated::WasmModule::new();
    let system_data = module.get___dandelion_system_data().unwrap();
    let system_data = format!("0x{:x}", system_data);
    // let system_data = syn::LitStr::new(&system_data, proc_macro2::Span::call_site());
    proc_macro::TokenStream::from_str(&system_data).unwrap()
}