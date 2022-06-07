#[allow(unused_variables)]
#[allow(unused_assignments)]

extern crate openssl;

use openssl::rsa::{Rsa, Padding};
use openssl::encrypt::{Encrypter, Decrypter};
use openssl::envelope::{Seal, Open};
use openssl::pkey::PKey;
use openssl::hash::MessageDigest;
use openssl::symm::Cipher;
use openssl::symm;
use openssl::rand;

fn open_test () {
	let rsa = Rsa::generate( 4096 ).unwrap();
	let key = PKey::from_rsa( rsa ).unwrap();

	let cipher = Cipher::chacha20_poly1305();
	let mut seal = Seal::new( cipher, &[key.clone()] ).unwrap();

	let secret: &mut [u8] = &mut [0; 8192];
	secret[ 0 ] = b'g';
	secret[ 1 ] = b'a';
	secret[ 2 ] = b'y';
	let mut encrypted = vec![0; secret.len() + cipher.block_size()];

	let mut enc_len = seal.update(secret, &mut encrypted).unwrap();
	enc_len += seal.finalize(&mut encrypted[enc_len..]).unwrap();
	encrypted.truncate(enc_len);

	println!( "{:?}", encrypted );
	println!( "{}", encrypted.len() );

	let enc_keys = seal.encrypted_keys();
	let iv = seal.iv();

	println!( "{}", enc_keys[0].len() );
	println!( "{}", iv.unwrap().len() );

	let mut opener = Open::new( cipher, &key, iv, &enc_keys[0] ).unwrap();

	let mut plain = vec![0; secret.len() + cipher.block_size()];
	let mut plainlen = opener.update( encrypted.as_slice(), &mut plain );
	plainlen = opener.finalize( &mut plain );
	println!( "{:?}", plain );
	println!( "{}", plain.len() );
}

fn pure_pkey_test () {
	let cipher = Cipher::chacha20_poly1305();

	let secret: &mut [u8] = &mut [0; 4096];
	for i in 0..secret.len() {
		secret[ i ] = i as u8;
	}

	// for AEAD
	const aad: &[u8] = b"WASSUP BTICHES";
	const aad2: &[u8] = b"bread";

	let mut symm_key = vec![0; cipher.key_len()];
	let mut symm_iv = vec![0; cipher.iv_len().unwrap()];
	let mut symm_tag = vec![0; 8];

	// Gen an RSA key
	let asym_key = {
		let rsa = Rsa::generate( 4096 ).unwrap();
		PKey::from_rsa( rsa ).unwrap()
	};

	// Symm Keygen
	rand::rand_bytes( &mut symm_key ).unwrap();
	rand::rand_bytes( &mut symm_iv ).unwrap();

	println!( "none: {:?}\npkcs1: {:?}\npkcs1 oaep: {:?}\npkcs1 pss:{:?}", Padding::NONE, Padding::PKCS1, Padding::PKCS1_OAEP, Padding::PKCS1_PSS );

	// Encrypt the plaintext using the symm cipher
	let symm_crypt = symm::encrypt_aead( cipher, &symm_key, Some( &symm_iv ), aad, secret, &mut symm_tag ).unwrap();
	// Encrypt the key using the rsa cipher
	let asym_crypt = {
		// Create a new buffer that holds the key + IV (could also do this initially)
		let mut plain_slice_of_bread: Vec<u8> = Vec::new();
		plain_slice_of_bread.append( &mut symm_key );
		plain_slice_of_bread.append( &mut symm_iv );
		//
		let mut asym_encrypter = Encrypter::new( &asym_key ).unwrap();
		asym_encrypter.set_rsa_padding( Padding::PKCS1_OAEP ).unwrap();
		asym_encrypter.set_rsa_oaep_md( MessageDigest::sha3_384() ).unwrap();
		println!( "{:?}", asym_encrypter.rsa_padding().unwrap() );

		let mut le_bouf = vec![0; asym_encrypter.encrypt_len( &plain_slice_of_bread ).unwrap()];
		assert!( asym_encrypter.encrypt( &plain_slice_of_bread, &mut le_bouf ).unwrap() == le_bouf.len() );
		le_bouf
	};

// 	println!( "{:?}", symm_crypt );
// 	println!( "{}", symm_crypt.len() );

	// Decrypt the key w/ rsa
	let (decr_key, decr_iv) = {
		let mut asym_decrypter = Decrypter::new( &asym_key ).unwrap();
		asym_decrypter.set_rsa_padding( Padding::PKCS1_OAEP ).unwrap();
		asym_decrypter.set_rsa_oaep_md( MessageDigest::sha3_384() ).unwrap();
		println!( "{:?}", asym_decrypter.rsa_padding().unwrap() );

		let mut recovered = vec![0; asym_decrypter.decrypt_len( &asym_crypt ).unwrap()];
		assert_eq!( asym_decrypter.decrypt( &asym_crypt, &mut recovered ).unwrap(), cipher.key_len() + cipher.iv_len().unwrap() );
		let dec_key: Vec<u8> = recovered.drain(..cipher.key_len()).collect();
		let dec_iv: Vec<u8> = recovered.drain(..cipher.iv_len().unwrap()).collect();
		(dec_key, dec_iv)
	};
	let plain = symm::decrypt_aead( cipher, &decr_key, Some( &decr_iv ), aad2, &symm_crypt, &symm_tag ).unwrap();

	println!( "{}", plain.len() );
	assert_eq!( secret, plain );
}

fn main () {
	pure_pkey_test();
}

fn join () {

}

