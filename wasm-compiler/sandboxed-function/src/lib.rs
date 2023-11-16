#![cfg_attr(not(test), no_std)]

// when compiling to a library, removes core's libc dependency
// on memset, memcpy, etc.
extern crate compiler_builtins;

/// This crate is a wrapper around the transpiled Rust code generated 
/// by rWasm. It serves the following purposes:
/// 
/// - provides a panic handler
/// - exposes entrypoints for Dandelion
/// - exposes wasm memory layout information to prepare inputs
/// - all the above must be exposed with #[no_mangle], which is not
///   allowed in #![forbid(unsafe_code)] crates, because it makes
///   linking unsafe

// need to define a panic handler, since this is a cdylib
use core::panic::PanicInfo;
#[cfg_attr(not(test), panic_handler)]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

mod interface;
use interface::_32_bit::DandelionSystemData;
use sandbox_generated::WasmModule;

#[no_mangle]
#[allow(unused)]
pub fn run(wasm_mem: &mut [u8]) -> Option<i32> {

    let mut instance = WasmModule::new(wasm_mem);
    let ret = instance._start();
    
    if ret.is_none() { 
        // the wasm module crashed
        return None; 
    }

    Some(0)
}

#[no_mangle]
#[allow(unused)]
pub const fn get_wasm_mem_size() -> usize {
    macro_utils::wasm_mem_size!() as usize
}

#[no_mangle]
#[allow(unused)]
pub const fn get_wasm_sdk_sysdata_offset() -> usize { 
    macro_utils::get___dandelion_system_data!() as usize
}

#[no_mangle]
#[allow(unused)]
pub const fn get_sdk_heap_base() -> usize {
    let wasm_heap_base = macro_utils::get___heap_base!() as usize;
    let wasm_mem_size = get_wasm_mem_size();
    // TODO this assumes that the heap comes last. I think this is always the case,
    //      but I'm not 100% sure, there are compiler options that change the
    //      memory layout (e.g. --stack-first). 
    //      Do not use the initial stack pointer here, as this is not the start
    //      of the heap if the code was compiled with --stack-first.
    let wasm_heap_size = wasm_mem_size - wasm_heap_base;
    let sdk_heap_size = get_sdk_heap_size();
    wasm_heap_base + wasm_heap_size - sdk_heap_size
}

#[no_mangle]
#[allow(unused)]
pub const fn get_sdk_heap_size() -> usize {
    macro_utils::sdk_heap_size!()
}

#[no_mangle]
#[allow(unused)]
pub fn sanity_check() -> i32 { 
    42
}


#[cfg(test)]
mod tests {
    // cdylib tests must be in the same file
    use super::*;

    #[test]
    fn mem_size() {
        let wasm_mem_size = get_wasm_mem_size();
        println!("wasm memory size: {}", wasm_mem_size);
        assert!(wasm_mem_size > 0);
    }
}