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
use interface::{_32_bit::DandelionSystemData, WasmInit, WasmStackInit, WasmHeapInit};
use sandbox_generated::WasmModule;


/// Allocate all wasm memory here, on the stack.
/// Disadvantage:
///      This requires copying the sdk heap and system data struct into 
///      wasm memory and back.
/// Advantage:
///      The WasmModule does not directly write to the buffer from
///      Dandelion, only to the one we allocate here.
fn run_alloc_mem_buf(init: WasmStackInit) -> Option<i32> {
    let WasmStackInit{dandelion_sdk_heap, sdk_system_data} = init;

    if dandelion_sdk_heap.len() > get_sdk_heap_size() {
        return Some(-1);
    }

    let mut wasm_mem = [0u8; get_wasm_mem_size()];

    // the global location of the SDK's __system_data struct in wasm memory
    let wasm_sysdata_struct_sdk: &mut DandelionSystemData = unsafe {
        &mut *(wasm_mem.as_mut_ptr().add(get_wasm_sdk_sysdata_offset()) as *mut DandelionSystemData)
    };

    // copy dandelion_sysdata into the SDK heap
    wasm_mem[get_sdk_heap_base()..].copy_from_slice(dandelion_sdk_heap);

    // copy the sdk system data struct to wasm memory
    *wasm_sysdata_struct_sdk = *sdk_system_data;

    // run the WasmModule
    let mut instance = WasmModule::new(&mut wasm_mem);
    let ret = instance._start();
    
    if ret.is_none() { 
        // the wasm module crashed
        return None; 
    }
    
    // copy the sdk heap back
    dandelion_sdk_heap.copy_from_slice(&wasm_mem[get_sdk_heap_base()..]);
    
    // copy the sdk system data struct back
    *sdk_system_data = *wasm_sysdata_struct_sdk;

    Some(0)
}

/// Use the buffer provided by Dandelion as wasm memory.
fn run_extern_mem_buf(init: WasmHeapInit) -> Option<i32> {
    let WasmHeapInit{wasm_mem} = init;
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
pub fn run(init: WasmInit) -> Option<i32> {
    match init {
        WasmInit::Stack(init) => run_alloc_mem_buf(init),
        WasmInit::Heap(init) => run_extern_mem_buf(init),
    }
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
    fn test_sanity_check() {
        assert_eq!(sanity_check(), 42);
    }

    #[test]
    fn mem_size() {
        let wasm_mem_size = get_wasm_mem_size();
        println!("wasm memory size: {}", wasm_mem_size);
        assert!(wasm_mem_size > 0);
    }

    #[test]
    fn sdk_heap_base() {
        let sdk_heap_base = get_sdk_heap_base();
        println!("sdk heap base: {}", sdk_heap_base);
        assert!(sdk_heap_base > 0);
    }
}