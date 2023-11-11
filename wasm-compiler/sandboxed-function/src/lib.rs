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

use interface::_32_bit::DandelionSystemData;
use sandbox_generated::WasmModule;

mod interface;

#[no_mangle]
#[allow(unused)]
pub fn run(dandelion_sdk_heap: &mut[u8], sdk_system_data: &mut DandelionSystemData) -> Option<i32> {

    // initialize the WasmModule
    let mut instance = WasmModule::new();

    if dandelion_sdk_heap.len() > get_sdk_heap_size() {
        return Some(-1);
    }

    // the global location of the SDK's __system_data struct in wasm memory
    let wasm_sysdata_struct_sdk: &mut DandelionSystemData = unsafe {
        &mut *(instance.get_memory().add(get_wasm_sdk_sysdata_offset()) as *mut DandelionSystemData)
    };

    // the SDK's heap in wasm memory
    let wasm_sdk_heap = unsafe{
        core::slice::from_raw_parts_mut(
            instance.get_memory().add(get_sdk_heap_base()),
            get_sdk_heap_size()
        )
    };

    // copy dandelion_sysdata into the SDK heap
    wasm_sdk_heap[..dandelion_sdk_heap.len()].copy_from_slice(dandelion_sdk_heap);

    // copy the sdk system data struct to wasm memory
    *wasm_sysdata_struct_sdk = *sdk_system_data;

    // run the WasmModule
    let ret = instance._start();
    if ret.is_none() { return None; }

    // copy the system data back
    dandelion_sdk_heap.copy_from_slice(wasm_sdk_heap);

    // copy the sdk system data struct back
    *sdk_system_data = *wasm_sysdata_struct_sdk;

    Some(0)
}

#[no_mangle]
#[allow(unused)]
pub fn get_wasm_sdk_sysdata_offset() -> usize { 
    macro_utils::get___dandelion_system_data!() as usize
}

#[no_mangle]
#[allow(unused)]
pub fn get_sdk_heap_size() -> usize {
    // TODO: make this configurable
    65536 * 1 // one wasm page
}

#[no_mangle]
#[allow(unused)]
pub fn get_sdk_heap_base() -> usize {
    let wasm_heap_base = macro_utils::get___heap_base!() as usize;
    let wasm_mem_size = macro_utils::get_memory_size!() as usize;
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
pub fn sanity_check() -> i32 { 
    42
}


#[cfg(test)]
mod tests {
    // cdylib tests must be in the same file

    use super::*;

    #[test]
    fn test_sanity_check() {
        assert_eq!(sanity_check(), 42);
    }
}