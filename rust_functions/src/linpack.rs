use ndarray::prelude::*;
use ndarray_linalg::Solve;

pub fn linpack(a: Array2<f64>, b: Array1<f64>) -> Array1<f64> {
    let c = a.solve_into(b).unwrap();
    c
}
