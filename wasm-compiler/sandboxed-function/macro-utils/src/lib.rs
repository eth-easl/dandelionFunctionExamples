extern crate sandbox_generated;
use sandbox_generated as Generated;

//
// Helper macros to extract data necessary for Dandelion from the generated
// code at compile time.
//

use konst::{unwrap_ctx, primitive::parse_usize};

const WASM_PAGE_SIZE: usize = 64 * 1024;
const WASM_MEM_SIZE: usize = unwrap_ctx!(parse_usize(env!("FUNCTION_WASM_MEM_SIZE")));
const SDK_HEAP_SIZE: usize = unwrap_ctx!(parse_usize(env!("FUNCTION_SDK_HEAP_SIZE")));

macro_rules! generate_getter {
    ($name:ident) => {
        #[proc_macro]
        #[allow(non_snake_case)]
        pub fn $name(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
            #[allow(unused_mut)]
            let mut mem_buf = &mut vec![0; WASM_MEM_SIZE];
            let module = Generated::WasmModule::new(&mut mem_buf);
            let $name = module.$name().unwrap();
            let $name = format!("0x{:x}", $name);
            $name.parse().unwrap()
        }
    }
}

generate_getter!(get___heap_base);
generate_getter!(get___dandelion_system_data);

#[proc_macro]
/// The wasm module's memory size in bytes.
pub fn wasm_mem_size(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
    (WASM_MEM_SIZE * WASM_PAGE_SIZE).to_string().parse().unwrap()
}

#[proc_macro]
// The sdk heap size in the wasm module memory, in bytes.
pub fn sdk_heap_size(_: proc_macro::TokenStream) -> proc_macro::TokenStream {
    (SDK_HEAP_SIZE * WASM_PAGE_SIZE).to_string().parse().unwrap()
}