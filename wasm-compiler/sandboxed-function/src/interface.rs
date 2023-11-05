pub use libc::{c_int, size_t, uintptr_t};

#[repr(C)]
pub struct DandelionSystemData {
    pub exit_code: c_int,
    pub heap_begin: uintptr_t,
    pub heap_end: uintptr_t,
    pub input_sets_len: size_t,
    pub input_sets: *const IoSetInfo,
    pub output_sets_len: size_t,
    pub output_sets: *const IoSetInfo,
    pub input_bufs: *const IoBufferDescriptor,
    pub output_bufs: *const IoBufferDescriptor,
}

#[repr(C)]
pub struct IoSetInfo {
    pub ident: uintptr_t,
    pub ident_len: size_t,
    pub offset: size_t,
}

#[repr(C)]
pub struct IoBufferDescriptor {
    pub ident: uintptr_t,
    pub ident_len: size_t,
    pub data: uintptr_t,
    pub data_len: size_t,
    pub key: size_t,
}