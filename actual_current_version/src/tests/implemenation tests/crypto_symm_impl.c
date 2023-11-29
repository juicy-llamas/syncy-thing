
#include <openssl/evp.h>
#include <openssl/err.h>
#include "root.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

// 	1 seems to mean success in OpenSSL
#define OPENSSL_TFAIL( fncall ) ({			\
	if ( ( fncall ) != 1 ) {				\
		ERR_print_errors_fp( stdout );		\
		return -1;							\
	}										\
})

/*
 * For any kind of crypto that relies on keys, we need to seed those keys with cryptographically random bytes
 * I've heard OpenSSL's PRNG is bad...and frankly it's harder to use anyways (this is a multithreaded program).
 * So I'm gonna make a micro random number generation library to this end.
 * For linux, this is very simple, getrandom avoids all complications
 */

ssize_t crypt_rand ( char* buffer, size_t size ) {
// 	getrandom is easy, and I fucking hope that no one is using a kernel below 3.5 for a secure application
	ssize_t bytes_written = 0;
	ssize_t error = 0;
// 	future: make this time out?
	while ( bytes_written < size ) {
		error = getrandom( buffer, size, 0 );
		if ( error == -1 && error != EINTR )
			return error;
		else if ( errno != EINTR )
			bytes_written += error;
	}
	return bytes_written;
}

/*
 * Test Goal: implement symmetric encryption and decryption.
 */

// I suppose
struct symm_ctx {
	char* key;
	EVP_MD_CTX* evpctx;
	char* encbuf;
	size_t encbuflen;
};

int symm_keygen ( struct symm_ctx* ctx ) {
	assert( ctx );
	assert( ctx->key );
	return -1 * ( crypt_rand( ctx->key, 256 ) < 0 );
}

int symm_keyload ( struct symm_ctx* ctx, const char* file_path ) {
	assert( ctx );
	assert( ctx->key );
}

// This will handle encryption and decryption in one fell swoop.
// The 'mode' parameter will also handle that
int symm_encrypt ( struct symm_ctx* ctx, const char* buffer, size_t length ) {
	char iv[ 96 ];
	assert( ctx );
	assert( buffer );

// 	initialize OpenSSL EVP ctx if one is needed
	if ( ctx->evpctx == 0 ) {
		ctx->evpctx = EVP_CIPHER_CTX_new();
		if ( ctx->evpctx == 0 ) {
			ERR_print_errors_fp( stdout );
			return -1;
		}

// 		also initialize a crypto context
		if ( crypt_rand( iv, 128 ) == -1 ) {
			EVP_CIPHER_CTX_free( ctx->evpctx );
			return -2;
		}

		if ( EVP_EncryptInit( ctx->evpctx, EVP_chacha20_poly1305(), ctx->key, iv ) == -1 ) {
			ERR_print_errors_fp( stdout );
			EVP_CIPHER_CTX_free( ctx->evpctx );
			return -3;
		}
	}

	OPENSSL_TFAIL
}

int symm_decrypt ( struct symm_ctx* ctx, const char* buffer, size_t length ) {
	assert( ctx );
	assert( buffer );

}
