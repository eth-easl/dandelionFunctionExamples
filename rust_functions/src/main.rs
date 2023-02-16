mod float_operation;
use float_operation::float_operations;
use ndarray::{Array1, Array2};

mod matmul;
use matmul::gen_matrix;
use matmul::gen_vector;
use matmul::matmul;

mod linpack;
use linpack::linpack;

mod aes_func;
use aes_func::aes_decrypt;
use aes_func::aes_encrypt;
use generic_array::GenericArray;

extern crate blas_src;

fn main() {
    let i = 5;
    println!("Running float operations");
    float_operations(i);
    println!("Running matmul");
    let n: usize = 128;
    let a = gen_matrix(n, n);
    let b = gen_matrix(n, n);
    let mut c = Array2::<f64>::zeros((n,n));
    c = matmul(a, b, c);
    println!("{:?}", c);

    println!("Running linpack");
    let a = gen_matrix(n, n);
    let b: Array1<f64>  = gen_vector(n);
//    let b = arr1(b.slice(s![0..1, ..]));
    let c = linpack(a, b);
    println!("{:?}", c);
// AES doesn't work yet :(
//    println!("Running AES");
//    let key = GenericArray::from([0u8; 16]);
//    let mut block = GenericArray::from([42u8; 16]);
//    block = aes_encrypt(key, &block);
//    println!("{:?}", block);
//    block = aes_decrypt(key, &block);
//    println!("{:?}", block);
}

