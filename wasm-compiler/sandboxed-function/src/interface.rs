pub use libc::{c_int, size_t, uintptr_t};

pub trait SizedIntTrait
where Self: Sized + Copy + Default
{}

pub mod _32_bit {
    pub type DandelionSystemData = super::DandelionSystemData<u32, u32>;
    impl super::SizedIntTrait for u32 {}
}

#[derive(Clone, Copy)]
#[repr(C)]
pub struct DandelionSystemData<PtrT: SizedIntTrait, SizeT: SizedIntTrait>
{
    pub exit_code: c_int,
    pub heap_begin: PtrT,
    pub heap_end: PtrT,
    pub input_sets_len: SizeT,
    pub input_sets: PtrT,
    pub output_sets_len: SizeT,
    pub output_sets: PtrT,
    pub input_bufs: PtrT,
    pub output_bufs: PtrT,
}

#[repr(C)]
pub struct IoSetInfo<PtrT: SizedIntTrait, SizeT: SizedIntTrait> {
    pub ident: PtrT,
    pub ident_len: SizeT,
    pub offset: SizeT,
}

#[repr(C)]
pub struct IoBufferDescriptor<PtrT: SizedIntTrait, SizeT: SizedIntTrait> {
    pub ident: PtrT,
    pub ident_len: SizeT,
    pub data: PtrT,
    pub data_len: SizeT,
    pub key: SizeT,
}