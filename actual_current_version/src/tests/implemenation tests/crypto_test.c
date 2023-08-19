
#include <openssl/evp.h>
#include <openssl/err.h>
#include "root.h"
#include <fcntl.h>
#include <unistd.h>


#define print_hash( hash ) printf( "%02x%02x%02x%02x" "%02x%02x%02x%02x" "%02x%02x%02x%02x" "%02x%02x%02x%02x" "%02x%02x%02x%02x" "%02x%02x%02x%02x" "%02x%02x%02x%02x" "%02x%02x%02x%02x", (unsigned char)( (hash)[ 0 ] ), (unsigned char)( (hash)[ 1 ] ), (unsigned char)( (hash)[ 2 ] ), (unsigned char)( (hash)[ 3 ] ), (unsigned char)( (hash)[ 4 ] ), (unsigned char)( (hash)[ 5 ] ), (unsigned char)( (hash)[ 6 ] ), (unsigned char)( (hash)[ 7 ] ), (unsigned char)( (hash)[ 8 ] ), (unsigned char)( (hash)[ 9 ] ), (unsigned char)( (hash)[ 10 ] ), (unsigned char)( (hash)[ 11 ] ), (unsigned char)( (hash)[ 12 ] ), (unsigned char)( (hash)[ 13 ] ), (unsigned char)( (hash)[ 14 ] ), (unsigned char)( (hash)[ 15 ] ), (unsigned char)( (hash)[ 16 ] ), (unsigned char)( (hash)[ 17 ] ), (unsigned char)( (hash)[ 18 ] ), (unsigned char)( (hash)[ 19 ] ), (unsigned char)( (hash)[ 20 ] ), (unsigned char)( (hash)[ 21 ] ), (unsigned char)( (hash)[ 22 ] ), (unsigned char)( (hash)[ 23 ] ), (unsigned char)( (hash)[ 24 ] ), (unsigned char)( (hash)[ 25 ] ), (unsigned char)( (hash)[ 26 ] ), (unsigned char)( (hash)[ 27 ] ), (unsigned char)( (hash)[ 28 ] ), (unsigned char)( (hash)[ 29 ] ), (unsigned char)( (hash)[ 30 ] ), (unsigned char)( (hash)[ 31 ] ) );
/*
 * Test goal: test if crypto works
 */

struct file_pointer {
	char* path;
// 	something like this should be used to identify which file structure the file is a part of
	int owner;
	FILE* fp;

};

struct crypt_ctx {
// 	OpenSSL requires this struct to be allocated by it, unfortunately
	EVP_MD_CTX* sslctx;
};

int crypt_init ( struct crypt_ctx* ptr ) {
	ptr->sslctx = EVP_MD_CTX_new();
	return ptr->sslctx == 0;
}

void crypt_free ( struct crypt_ctx* ptr ) {
	EVP_MD_CTX_free( ptr->sslctx );
}

// 	1 seems to mean success in OpenSSL
#define OPENSSL_TFAIL( fncall ) ({			\
	if ( ( fncall ) != 1 ) {				\
		ERR_print_errors_fp( stdout );		\
		return -1;							\
	}										\
})

// Hashing is a very important task that we will probably routinely do to verify file integrity.
// It is also a handy way to identify files uniquely and succinctly.
// OUTPUT SHOULD HOLD THE HASH (sha256 == 32 bytes)
// NOTE: ****could fail for very large buffers****
int crypt_hash_buffer ( struct crypt_ctx* ctx, const unsigned char* buffer, size_t size, unsigned char* output ) {
	unsigned int hsz = 32;
	assert( ctx->sslctx );
	assert( output );
	assert( buffer );

	OPENSSL_TFAIL( EVP_DigestInit_ex( ctx->sslctx, EVP_sha256(), 0 ) );
	OPENSSL_TFAIL( EVP_DigestUpdate( ctx->sslctx, buffer, size ) );
	OPENSSL_TFAIL( EVP_DigestFinal_ex( ctx->sslctx, output, &hsz ) );
// 	sort of a hack to ensure it's 32
	OPENSSL_TFAIL( hsz == 32 );
// 	reset ctx for next use
	OPENSSL_TFAIL( EVP_MD_CTX_reset( ctx->sslctx ) );
	return 0;
}

// Same thing but uses a stream instead of a buffer
int crypt_hash_file ( struct crypt_ctx* ctx, FILE* fp, unsigned char* output ) {
	// 	read in 4KB chunks
	char buffer[ 4096 ];
	unsigned int size = 0;
	unsigned int hsz = 32;
	assert( ctx->sslctx );
	assert( output );
	assert( fp );

	OPENSSL_TFAIL( EVP_DigestInit_ex( ctx->sslctx, EVP_sha256(), 0 ) );
	while ( ( size = fread( buffer, 1, 4096, fp ) ) > 0 )
		OPENSSL_TFAIL( EVP_DigestUpdate( ctx->sslctx, buffer, size ) );
// 	final update
	OPENSSL_TFAIL( EVP_DigestUpdate( ctx->sslctx, buffer, size ) );

// 	file io error
	if ( size == 0 && !( feof( fp ) ) ) {
		log_err( "file error" );
		return -2;
	}

	OPENSSL_TFAIL( EVP_DigestFinal_ex( ctx->sslctx, output, &hsz ) );
	// 	sort of a hack to ensure it's 32
	OPENSSL_TFAIL( hsz == 32 );
	// 	reset ctx for next use
	OPENSSL_TFAIL( EVP_MD_CTX_reset( ctx->sslctx ) );

	return 0;
}

int main () {
	struct crypt_ctx ctx;
	crypt_init( &ctx );

	puts( "Creating file..." );
// 	alternatively something random
	const unsigned char stuff_to_hash[] = "";
	size_t stuff_size = 0; // sizeof( stuff_to_hash );
	printf( "Hashing stuff: '%s', size: %li\n", stuff_to_hash, stuff_size );
// 	alternatively a number
	FILE* fp = fopen( "a_file", "w" );
	fwrite( stuff_to_hash, stuff_size, 1, fp );
	fclose( fp );

	// 	base reference
	puts( "Hashing reference..." );
	unsigned char hash_text[ 64 ] = {0};
	system( "openssl sha256 a_file | grep -oE \"[A-Za-z0-9]{64}\" >> a_file_hash" );
	fp = fopen( "a_file_hash", "r" );
	fread( hash_text, 64, 1, fp );

	unsigned char hash[ 32 ] = {0};
	for ( int i = 0; i < 32; i++ ) {
		unsigned char ch1 = hash_text[ i << 1 ] > '9' ? hash_text[ i << 1 ] - 'a' + 10 : hash_text[ i << 1 ] - '0';
		unsigned char ch2 = hash_text[ ( i << 1 ) + 1 ] > '9' ? hash_text[ ( i << 1 ) + 1 ] - 'a' + 10 : hash_text[ ( i << 1 ) + 1 ] - '0';
		hash[ i ] = ( ch1 << 4 ) + ch2;
	}

// 	buffer fn
	puts( "Hashing buffer..." );
	unsigned char hash_two[ 32 ] = {0};
	crypt_hash_buffer( &ctx, stuff_to_hash, stuff_size, hash_two );
	print_hash( hash_two );
	puts( "" );
	for ( int i = 0; i < 32; i++ ) {
		if ( hash_two[ i ] != hash[ i ] ) {
			printf( "Hashes mismatched at index %i! Correct: ", i );
			print_hash( hash );
			printf( ", Actual: " );
			print_hash( hash_two );
			puts( "" );
			break;
		}
	}

	// 	fp fn
	puts( "Hashing file..." );
	memset( hash_two, 0, 32 );
	fp = fopen( "a_file", "r" );
	crypt_hash_file( &ctx, fp, hash_two );
	print_hash( hash_two );
	puts( "" );
	for ( int i = 0; i < 32; i++ ) {
		if ( hash_two[ i ] != hash[ i ] ) {
			printf( "Hashes mismatched at index %i! Correct: ", i );
			print_hash( hash );
			printf( ", Actual: " );
			print_hash( hash_two );
			puts( "" );
			break;
		}
	}
	fclose( fp );

	unlink( "a_file_hash" );
	unlink( "a_file" );
	crypt_free( &ctx );
}
