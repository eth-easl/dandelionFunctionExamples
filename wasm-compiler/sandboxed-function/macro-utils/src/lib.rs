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
            let module = Generated::WasmModule::new();
            let $name = module.$name().unwrap();
            let $name = format!("0x{:x}", $name);
            $name.parse().unwrap()
        }
    }
}

generate_getter!(get___dandelion_system_data);
generate_getter!(get_INTERFACE_MEM_SIZE_FOR_WASM);
generate_getter!(get_INTERFACE_MEM_FOR_WASM);