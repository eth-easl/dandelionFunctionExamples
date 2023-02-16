pub fn float_operations(n: i64) {
    for i in 0..n {
        let i: f64 = i as f64;
        let sin_i = i.sin();
        let cos_i = i.cos();
        let sqrt_i = i.sqrt();

        //println!("{n}: {sin_i} {cos_i} {sqrt_i}");
        println!("{0}: {1} {2} {3}", i, sin_i, cos_i, sqrt_i);
    }
}


