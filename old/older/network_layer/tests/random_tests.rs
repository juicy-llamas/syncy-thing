extern crate openssl;

#[cfg(test)]
mod tests {
	use openssl::rsa::Rsa;
	use openssl::envelope::Seal;
	use openssl::pkey::PKey;
	use openssl::symm::Cipher;

	#[test]
    fn offload () {
		assert!( true );
	}
}
