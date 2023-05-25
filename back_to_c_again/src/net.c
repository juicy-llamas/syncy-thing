
#include "net-internal.h"
#include "net.h"

// // IM A STWING
// struct Stwing {
// 	// the string
// 	char* string;
// 	// length, includes the null character
// 	unsigned int length;
// 	// to take up the space. Could be for things like wchar_t vs char, or different encodings.
// 	unsigned int flags;
// };
//
// int net_init () {
// 	struct protoent* protn = getprotobyname( "TCP" );
//
// 	if ( protn )
// 		TCP_PROT = protn->p_proto;
// 	else {
// 		log_warn( "getprotobyname failed, defaulting to 6." );
// 		TCP_PROT = 6;
// 		return 1;
// 	}
//
// 	if ( TCP_PROT != 6 )
// 		log_warn( "TCP_PROT is not 6" );
//
// 	return 0;
// }
//
// int send_file ( int sock, struct Stwing file ) {
// 	char buff[ BUFFLEN ];
// 	FILE* fp = fopen( file->string, "r" );
// 	unsigned int bytes_to_send = 0;
// 	int error = 0;
//
// 	if ( fp == NULL )
// 		log_abort( "file failed to open" );
//
// 	while ( ( bytes_to_send = fread( buff, BUFFLEN, 1, fp ) ) != 0 ) {
// 		if ( ( error = writeplus( sock, buff, bytes_to_send ) ) < 0 )
// 			break;
// 	}
//
// 	if ( error ) {
// 		fprintf( stderr, "send_file: socket failed to write %i", error );
// 		return -1;
// 	} else if ( ( error = ferror( fp ) ) != 0 ) {
// 		fprintf( stderr, "send_file: file failed to read with error %i", error );
// 		return -2;
// 	}
//
// 	fclose( k );
// 	return 0;
// }
//
// int recv_file ( int sock, int bytes, struct Stwing file ) {
// 	char buff[ BUFFLEN ];
// 	FILE* fp = fopen( file->string, "r" );
// 	unsigned int bytes_to_send = 0;
// 	int error = 0;
//
// 	if ( fp == NULL )
// 		log_abort( "file failed to open" );
//
// 	while ( ( error = recvplus( sock, buff, bytes_to_send ) ) < 0 ) {
// 		if (  )
// 			break;
// 	}
//
// 	if ( error ) {
// 		fprintf( stderr, "send_file: socket failed to write %i", error );
// 		return -1;
// 	} else if ( ( error = ferror( fp ) ) != 0 ) {
// 		fprintf( stderr, "send_file: file failed to read with error %i", error );
// 		return -2;
// 	}
//
// 	fclose( k );
// 	return 0;
// }
