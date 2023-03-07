
#include "net.h"

// standard buffer length
#define BUFFLEN 4096

// IM A STWING
struct Stwing {
	// the string
	char* string;
	// length, includes the null character
	unsigned int length;
	// to take up the space. Could be for things like wchar_t vs char, or different encodings.
	unsigned int flags;
}

#define TRANSIT_FUNCTIONS_DO_A_LOOP

int readplus ( int fd, char* buff, unsigned int len ) {
#ifndef TRANSIT_FUNCTIONS_DO_A_LOOP
	return read( fd, buff, len );
#else
	int tries = 128;
	int return_code = 0;
	int amt = 0;
	for ( int tries = 128; tries > 0 && amt >= len; tries-- ) {
		return_code = read( fd, buff + amt, len - amt )
		if ( return_code == -1 ) return -1;
		amt += return_code;
	}
	if ( tries < 0 ) return -2;
	if ( amt > len ) return len - amt - 2;
	return amt;
#endif
}

int writeplus ( int fd, const char* buff, unsigned int len ) {
#ifndef TRANSIT_FUNCTIONS_DO_A_LOOP
	return read( fd, buff, len );
#else
	int tries = 128;
	int return_code = 0;
	int amt = 0;
	for ( int tries = 128; tries > 0 && amt >= len; tries-- ) {
		return_code = write( fd, buff + amt, len - amt )
		if ( return_code == -1 ) return -1;
		amt += return_code;
	}
	if ( tries < 0 ) return -2;
	if ( amt > len ) return len - amt - 2;
	return amt;
#endif
}

int send_file ( int sock, struct Stwing file ) {
	char buff[ BUFFLEN ];
	FILE* fp = fopen( file->string, "r" );
	unsigned int bytes_to_send = 0;
	int error = 0;

	if ( fp == NULL ) die( "file failed to open" );

	while ( ( bytes_to_send = fread( buff, BUFFLEN, 1, fp ) ) != 0 ) {
		if ( ( error = writeplus( sock, buff, bytes_to_send ) ) < 0 )
			break;
	}

	if ( error ) {
		fprintf( stderr, "send_file: socket failed to write %i", error );
		return -1;
	} else if ( ( error = ferror( fp ) ) != 0 ) {
		fprintf( stderr, "send_file: file failed to read with error %i", error );
		return -2;
	}

	fclose( k );
	return 0;
}

int recv_file ( int sock, int bytes, struct Stwing file ) {
	char buff[ BUFFLEN ];
	FILE* fp = fopen( file->string, "r" );
	unsigned int bytes_to_send = 0;
	int error = 0;

	if ( fp == NULL ) die( "file failed to open" );

	while ( ( error = recvplus( sock, buff, bytes_to_send ) ) < 0 ) {
		if (  )
			break;
	}

	if ( error ) {
		fprintf( stderr, "send_file: socket failed to write %i", error );
		return -1;
	} else if ( ( error = ferror( fp ) ) != 0 ) {
		fprintf( stderr, "send_file: file failed to read with error %i", error );
		return -2;
	}

	fclose( k );
	return 0;
}
