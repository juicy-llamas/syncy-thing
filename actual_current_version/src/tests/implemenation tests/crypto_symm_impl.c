
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

enum symm_crypt_mode {
	scm_encrypt = -1,
	scm_nochange = 0,
	scm_decrypt = 1
};

/*
 * First,
 */

/*
 * Test Goal: implement symmetric encryption and decryption.
 */

struct symm_ctx {
	char* key;
	EVP_MD_CTX* evpctx;
	char* encbuf;
	size_t encbuflen;
};

struct symm_ctx* symm_keygen ( struct symm_ctx* ctx ) {
	assert( ctx );
	symm_ctx->
}

struct symm_ctx* symm_keyload ( struct symm_ctx* ctx, const char* key ) {
	assert( ctx );

}

// This will handle encryption and decryption in one fell swoop.
// The 'mode' parameter will also handle that
struct symm_ctx* symm_crypt ( struct symm_ctx* ctx, const char* buffer, size_t length, enum symm_crypt_mode mode ) {
	assert( ctx );

// 	initialize OpenSSL EVP ctx if one is needed
	if ( ctx->evpctx == 0 ) {
		ctx->evpctx = EVP_MD_CTX_new();
		if ( ctx->evpctx == 0 ) {
			ctx->evpctx = -1;
			return 0;
		}

// 		also initialize a crypto context
		OPENSSL_TFAIL( EVP_CipherInit_ex(
	}

	OPENSSL_TFAIL
}

int symm_decrypt () {

}
