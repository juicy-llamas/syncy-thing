
extern crate openssl;

use openssl::rsa::Rsa;
use openssl::envelope::{Seal, Open};
use openssl::pkey::PKey;
use openssl::symm::Cipher;

fn main () {
	let rsa = Rsa::generate( 4096 ).unwrap();
	let key = PKey::from_rsa( rsa ).unwrap();

	let cipher = Cipher::chacha20_poly1305();
	let mut seal = Seal::new( cipher, &[key] ).unwrap();

	let secret: &[u8] = &[0; 8192];
	let mut encrypted = vec![0; secret.len() + cipher.block_size()];

	let mut enc_len = seal.update(secret, &mut encrypted).unwrap();
	enc_len += seal.finalize(&mut encrypted[enc_len..]).unwrap();
	encrypted.truncate(enc_len);

	println!( "{:?}", encrypted );
	println!( "{}", encrypted.len() == secret.len() );

	let enc_keys = seal.encrypted_keys();
	let iv = seal.iv();

	println!( "{}", enc_keys[0].len() );
	println!( "{}", iv.unwrap().len() );

// 	let mut opener = Open::new( cipher ).unwrap();
}

fn join () {

}

