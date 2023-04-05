use ndarray::prelude::*;
use ndarray::{Array};
use ndarray_rand::RandomExt;
use ndarray_rand::rand_distr::Uniform;

pub fn matmul(a: Array2<f64>, b: Array2<f64>, mut c: Array2<f64>) -> Array2<f64> {
    let (n_a, m_a) = a.dim();
    let (n_b, m_b) = b.dim();
    let (n_c, m_c) = c.dim();
    if m_a != n_b { panic!{"matrix mismatch! m_a != n_b"}; }
    if m_c != m_b { panic!{"matrix mismatch! m_c != m_b"}; }
    if n_c != n_a { panic!{"matrix mismatch! n_c != n_a"}; }
    
    c = a.dot(&b);
    c
}    

fn print_type_of<T>(_: &T) {
        println!("{}", std::any::type_name::<T>())
}


pub fn gen_matrix(n: usize, m: usize) -> Array<f64, Dim<[usize; 2]>> {
    let a = Array::random((n, m), Uniform::new(-0.5, 0.5));
    print_type_of(&a);
    //    let a = Array::<f64, Ix2>::zeros((n, n).f());
    a
}

pub fn gen_vector(n: usize) -> Array1<f64> {
    let a = Array::random(n, Uniform::new(-0.5, 0.5));
    print_type_of(&a);
    a
}
