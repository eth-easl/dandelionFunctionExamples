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


// a wrapper around WasmModule
/*
mod wrapper {
    extern crate sandbox_generated;

    pub struct WasmModule(pub sandbox_generated::WasmModule);

    impl WasmModule {
        #[no_mangle]
        pub const fn new() -> Self {
            Self (sandbox_generated::WasmModule::new())
        }
        #[no_mangle]
        pub fn run(&mut self) -> Option<()> {
            self.0._start()
        }
    }
}
 */


#[no_mangle]
#[allow(unused)]
pub fn run() -> Option<()> {
    let mut instance = WasmModule::new();
    instance._start()
}

#[no_mangle]
#[allow(unused)]
pub fn get_sysdata_wasm_mem_offset() -> usize { 
    macro_utils::get_INTERFACE_MEM_FOR_WASM!() as usize
}

#[no_mangle]
#[allow(unused)]
pub fn sanity_check() -> i32 { 
    // sandbox_generated::WasmModule::new();
    43
}