#![cfg_attr(not(test), no_std)]

// when compiling to a binary, to make the linker happy
// who expects a C runtimme compliant interface
// alternatively: compile to a suitable bare-metal target
/*
#![feature(start)]

extern crate libc;

#[start]
fn start(_: isize, _: *const *const u8) -> isize { 42 }
*/


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

// static exports for Dandelion to prepare the inputs for the WasmModule
// it seems like I cannot use this as LLVM doesn't seem to support global
// variables in PIC for ARM targets
/*
#[no_mangle]
pub static SYSDATA_MEM_START_IDX: usize = 42; //macro_utils::get_INTERFACE_MEM_FOR_WASM!() as usize;

#[no_mangle]
pub static mut INSTANCE: wrapper::WasmModule = wrapper::WasmModule::new();
#[no_mangle]
pub static SYSDATA_MEM_START_IDX: usize = unsafe { 
    match INSTANCE.0.get_INTERFACE_MEM_FOR_WASM() {
        Some(x) => x as usize,
        None => panic!("cannot read SYSDATA_MEM_START"),
    
    }
};
#[no_mangle]
pub static SYSDATA_MEM_SIZE: usize = unsafe { 
    match INSTANCE.0.get_INTERFACE_MEM_SIZE_FOR_WASM() {
        Some(x) => x as usize,
        None => panic!("cannot read SYSDATA_MEM_SIZE"),
    }
};
#[no_mangle]
pub static SYSTEM_DATA_IDX: usize = unsafe { 
    match INSTANCE.0.get___dandelion_system_data() {
        Some(x) => x as usize,
        None => panic!("cannot read SYSTEM_DATA_IDX"),
    }
};
 */


mod interface;


/*fn move_sysdata_to_region_and_duplicate_struct(
    dandelion_sysdata: &mut [u8],
    wasm_sysdata_region: &mut [u8],
    wasm_sysdata_struct_sdk: &mut DandelionSystemData,
) {
    // (1) copy all system data into the pre-allocated region in wasm memory

    // notice that the pointers in the system data are valid because
    // Dandelion respected the memory layout of the WasmModule
    // via get_sysdata_wasm_mem_offset()

    wasm_sysdata_region.copy_from_slice(dandelion_sysdata);

    // (2) duplicate the DandelionSystemData struct in wasm memory where
    //   the __system_data struct of the function SDK is located

    // the memory for the __system_data struct of the function SDK is stored
    // somewhere else in wasm memory
    // we need to copy the DandelionSystemData struct from dandelion_sysdata
    // to the __system_data struct in wasm memory, and fix the pointers
    // in that struct to point to the correct locations in the memory we
    // just copied

    // TODO: we can avoid this by changing the function sdk such that
    //       __system_data is a _pointer_ to the DandelionSystemData struct

    // extract DandelionSystemData struct from dandelion_sysdata
    let dandelion_sysdata_struct: &mut DandelionSystemData = unsafe {
        &mut *(dandelion_sysdata.as_mut_ptr() as *mut DandelionSystemData)
    };

    // convert to a slice of bytes
    let dandelion_sysdata_struct_region: &mut [u8] = unsafe {
        core::slice::from_raw_parts_mut(
            dandelion_sysdata_struct as *mut DandelionSystemData as *mut u8,
            core::mem::size_of::<DandelionSystemData>()
        )
    };

    // convert to a slice of bytes
    let wasm_sysdata_struct_region: &mut [u8] = unsafe {
        core::slice::from_raw_parts_mut(
            wasm_sysdata_struct_sdk as *mut DandelionSystemData as *mut u8,
            core::mem::size_of::<DandelionSystemData>()
        )
    };

    // copy the DandelionSystemData struct to the SDK's __system_data struct
    wasm_sysdata_struct_region.copy_from_slice(dandelion_sysdata_struct_region);

    // (3) fix the pointers in the SDK's __system_data struct to point to the data
    //   we put into the pre-allocated system data region in wasm memory

    // the offset we need to add to the pointers in the __system_data struct
    let sysdata_ptr_offset = get_wasm_sdk_sysdata_offset() - get_wasm_sysdata_region_offset();

    macro_rules! add_offset {
        ($ptr:expr, $type:ty) => {
            $ptr = unsafe {
                ($ptr as *const u8).add(sysdata_ptr_offset) as $type
            }
        }
    }
    
    add_offset!(wasm_sysdata_struct_sdk.heap_begin,     uintptr_t);
    add_offset!(wasm_sysdata_struct_sdk.heap_end,       uintptr_t);
    add_offset!(wasm_sysdata_struct_sdk.input_sets,     *const IoSetInfo);
    add_offset!(wasm_sysdata_struct_sdk.output_sets,    *const IoSetInfo);
    add_offset!(wasm_sysdata_struct_sdk.input_bufs,     *const IoBufferDescriptor);
    add_offset!(wasm_sysdata_struct_sdk.output_bufs,    *const IoBufferDescriptor);
}*/


#[no_mangle]
#[allow(unused)]
pub fn run(dandelion_sysdata: &mut[u8], sdk_system_data: &mut DandelionSystemData) -> Option<i32> {

    /* old version which doesn't need a DandelionSystemData struct

    // (0) initialize the WasmModule

    let mut instance = WasmModule::new();

    if dandelion_sysdata.len() > get_wasm_sysdata_region_size() {
        return None;
    }

    let wasm_sysdata_region = unsafe{
        core::slice::from_raw_parts_mut(
            instance.get_memory().add(get_wasm_sysdata_region_offset()),
            dandelion_sysdata.len()
        )
    };

    // the global location of the SDK's __system_data struct in wasm memory
    let wasm_sysdata_struct_sdk: &mut DandelionSystemData = unsafe {
        &mut *(instance.get_memory().add(get_wasm_sdk_sysdata_offset()) as *mut DandelionSystemData)
    };

    // (1) copy system data into wasm module's memory

    move_sysdata_to_region_and_duplicate_struct(
        dandelion_sysdata, 
        wasm_sysdata_region, 
        wasm_sysdata_struct_sdk
    );

    // sanity checks

    // the DandelionSystemData struct at the start of the pre-allocated
    // system data region in wasm memory
    let wasm_sysdata_struct_in_region = unsafe {
        &mut *(instance.get_memory().add(get_wasm_sysdata_region_offset()) as *mut DandelionSystemData)
    };

    assert_eq!(wasm_sysdata_struct_in_region.exit_code,         wasm_sysdata_struct_sdk.exit_code);
    assert_eq!(wasm_sysdata_struct_in_region.heap_begin,        wasm_sysdata_struct_sdk.heap_begin);
    assert_eq!(wasm_sysdata_struct_in_region.heap_end,          wasm_sysdata_struct_sdk.heap_end);
    assert_eq!(wasm_sysdata_struct_in_region.input_sets_len,    wasm_sysdata_struct_sdk.input_sets_len);
    assert_eq!(wasm_sysdata_struct_in_region.input_sets,        wasm_sysdata_struct_sdk.input_sets);    // should point to the same location
    assert_eq!(wasm_sysdata_struct_in_region.output_sets_len,   wasm_sysdata_struct_sdk.output_sets_len);
    assert_eq!(wasm_sysdata_struct_in_region.output_sets,       wasm_sysdata_struct_sdk.output_sets);   // should point to the same location
    assert_eq!(wasm_sysdata_struct_in_region.input_bufs,        wasm_sysdata_struct_sdk.input_bufs);    // should point to the same location
    assert_eq!(wasm_sysdata_struct_in_region.output_bufs,       wasm_sysdata_struct_sdk.output_bufs);   // should point to the same location


    // (2) run the WasmModule

    let ret = instance._start();

    // (3) copy the system data back

    // notice the __system_data struct in wasm memory doesn't change
    // so we can just copy the one from the pre-allocated sysdata wasm memory

    dandelion_sysdata.copy_from_slice(wasm_sysdata_region);

    ret
    */

    // (0) initialize the WasmModule

    let mut instance = WasmModule::new();

    if dandelion_sysdata.len() > get_wasm_sysdata_region_size() {
        return None;
    }

    // the pre-allocated region for system data in wasm memory
    let wasm_sysdata_region: &mut [u8] = unsafe{
        core::slice::from_raw_parts_mut(
            instance.get_memory().add(get_wasm_sysdata_region_offset()),
            dandelion_sysdata.len()
        )
    };

    // the global location of the SDK's __system_data struct in wasm memory
    let wasm_sysdata_struct_sdk: &mut DandelionSystemData = unsafe {
        &mut *(instance.get_memory().add(get_wasm_sdk_sysdata_offset()) as *mut DandelionSystemData)
    };

    // (1) copy dandelion_sysdata into the pre-allocated region in wasm memory

    wasm_sysdata_region[..dandelion_sysdata.len()].copy_from_slice(dandelion_sysdata);

    // (2) copy the sdk system data struct to wasm memory

    *wasm_sysdata_struct_sdk = *sdk_system_data;

    // (3) run the WasmModule

    let ret = instance._start();

    // (4) copy the system data back

    dandelion_sysdata.copy_from_slice(wasm_sysdata_region);

    // (5) copy the sdk system data struct back

    *sdk_system_data = *wasm_sysdata_struct_sdk;

    Some(sdk_system_data.exit_code)

}

#[no_mangle]
#[allow(unused)]
pub fn get_wasm_sysdata_region_offset() -> usize { 
    macro_utils::__wasm_sysdata_region_base!() as usize
}

#[no_mangle]
#[allow(unused)]
pub fn get_wasm_sysdata_region_size() -> usize { 
    macro_utils::__wasm_sysdata_region_size!() as usize
}

#[no_mangle]
#[allow(unused)]
pub fn get_wasm_sdk_sysdata_offset() -> usize { 
    macro_utils::get___dandelion_system_data!() as usize
}

#[no_mangle]
#[allow(unused)]
pub fn get_sdk_heap_size() -> usize {
    macro_utils::__wasm_sdk_heap_size!() as usize * 64 * 1024
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
    use super::*;

    #[test]
    fn test_sanity_check() {
        assert_eq!(sanity_check(), 42);
    }
    #[test]
    fn test_wasm_mem_size2() {
        let region_size = get_wasm_sysdata_region_size();
        let sdk_heap_size = get_sdk_heap_size();
        let sdk_heap_base = get_sdk_heap_base();
        assert!(region_size > 0);
    }
}