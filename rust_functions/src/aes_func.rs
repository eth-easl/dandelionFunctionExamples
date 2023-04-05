use aes::Aes128;
use aes::cipher::{
        BlockCipher, BlockEncrypt, BlockDecrypt, KeyInit,
};
use generic_array::GenericArray;
use generic_array::ArrayLength;

pub fn aes_encrypt(key: GenericArray<u8, N: ArrayLength<u8>>, 
                   block: GenericArray<u8, N: ArrayLength<u8>>) 
    -> GenericArray<u8, N: ArrayLength<u8>> {
    let cipher = Aes128::new(&key);

    cipher.encrypt_block(&mut block);

    block
}

pub fn aes_decrypt(key: GenericArray<u8, N: ArrayLength<u8>>, 
                   block: GenericArray<u8, N: ArrayLength<u8>>) 
    -> GenericArray<u8, N: ArrayLength<u8>> {
    let cipher = Aes128::new(&key);

    cipher.decrypt_block(&mut block);

    block
}

