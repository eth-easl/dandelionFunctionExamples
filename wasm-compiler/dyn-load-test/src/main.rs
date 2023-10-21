use std::{*, io::Read};
use libc::mmap;
mod elf_parser;

fn read_so_and_call(obj_file_path: &str, entry_point: &str, expected_result: i32) {
    let mut file = fs::File::open(obj_file_path).unwrap();
    let mut code = Vec::new();
    file.read_to_end(&mut code).unwrap();

    // read elf header
    let elf = elf_parser::ParsedElf::new(&code).unwrap();

    // get pointer to symbol get_data
    let (entry, length) = elf.get_symbol_by_name(&code, entry_point).unwrap();
    let code = &code[entry..entry + length];

    /*
        for the "run() -> i32 { 42 }" function, the disassembly should be:
        objdump --disassemble=run guest/target/release/libguest.so

            0xb8 0x2a 0x0 0x0 0x0       mov    eax,0x2a
            0xc3                        ret
     */

    let result = unsafe {
        // https://users.rust-lang.org/t/how-can-i-execute-hex-opcodes-directly-in-rust/31123/13
        let addr = 
            mmap(
                0 as *mut _,
                code.len(),
                libc::PROT_READ | libc::PROT_WRITE | libc::PROT_EXEC,
                libc::MAP_PRIVATE | libc::MAP_ANONYMOUS,
                -1,
                0,
            );
        assert_ne!(addr, libc::MAP_FAILED);
        std::ptr::copy_nonoverlapping(code.as_ptr(), addr as *mut u8, code.len());
        let get_data: extern "C" fn() -> i32 = std::mem::transmute(addr);
        get_data()
    };
    
    println!("result: {}", result);
    assert_eq!(result, expected_result);
}

// // include ./guest/target/release/libguest.so
// static CODE: &'static [u8] = include_bytes!("../guest/target/release/libguest.so");

fn main() {
    read_so_and_call("guest/target/release/libguest.so", "run", 42);
}
