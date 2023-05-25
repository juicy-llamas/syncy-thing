
#include "net.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define DBG_ERR_MSG( msg, ... ) ({									\
	char* errnostr = strerror( errno );								\
	fprintf( stderr, "%s %i: " msg ". Errno says %s\n", __FILE__, 	\
			 __LINE__, ##__VA_ARGS__, errnostr );					\
})

// standard buffer length
#define BUFFLEN 4096

// again something different here
#define PORT 6666

// IM A STWING
struct Stwing {
	// the string
	char* string;
	// length, includes the null character
	unsigned int length;
	// to take up the space. Could be for things like wchar_t vs char, or different encodings.
	unsigned int flags;
};

#define TRANSIT_FUNCTIONS_DO_A_LOOP

int readplus ( int fd, char* buff, unsigned int len ) {
#ifndef TRANSIT_FUNCTIONS_DO_A_LOOP
	return read( fd, buff, len );
#else
	int tries = 128;
	int return_code = 0;
	unsigned int amt = 0;
	for ( int tries = 128; tries > 0 && amt >= len; tries-- ) {
		return_code = read( fd, buff + amt, len - amt );
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
	unsigned int amt = 0;
	for ( int tries = 128; tries > 0 && amt >= len; tries-- ) {
		return_code = write( fd, buff + amt, len - amt );
		if ( return_code == -1 ) return -1;
		amt += return_code;
	}
	if ( tries < 0 ) return -2;
	if ( amt > len ) return len - amt - 2;
	return amt;
#endif
}

int send_file ( int sock, struct Stwing* file ) {
	char buff[ BUFFLEN ];
	FILE* fp = 0;
	int bytes_to_send = 0;
	int bytes_read = 0;
	int error = 0;

// 	open file and get length (seek end and then beginning)
	fp = fopen( file->string, "r" );
	if ( fp == NULL ) return -3;
	if ( ( bytes_to_send = fseek( fp, 0, SEEK_END ) ) == -1 ) return -4;
	if ( fseek( fp, 0, SEEK_SET ) == -1 ) return -5;

// 	send bytes in file
	*((int*)buff) = bytes_to_send;
	if ( ( error = writeplus( sock, buff, sizeof( int ) ) ) < 0 )
		return error;

// 	read and send 4K buffers
	while ( ( bytes_read = fread( buff, BUFFLEN, 1, fp ) ) != 0 ) {
		if ( ( error = writeplus( sock, buff, bytes_read ) ) < 0 )
			break;
		memset( buff, 0, BUFFLEN );
	}

	if ( error ) {
		DBG_ERR_MSG( "send_file: failed to read file with %i", error );
		return -1;
	} else if ( ( error = ferror( fp ) ) != 0 ) {
		DBG_ERR_MSG( "send_file: socket failed to read with %i", error );
		return -2;
	}

	fclose( fp );
	return 0;
}

int recv_file ( int sock, int bytes, struct Stwing* file ) {
	char buff[ BUFFLEN ];
	FILE* fp = fopen( file->string, "w" );
	int bytes_to_recv = 0;
	int bytes_to_read = 0;
	int error = 0;

	if ( fp == NULL ) return -3;

	if ( ( error = readplus( sock, buff, sizeof( int ) ) ) > 0 ) return -4;
	bytes_to_recv = *((int*)buff);

	while ( bytes_to_recv > 0 ) {
		bytes_to_read = bytes_to_recv > BUFFLEN ? BUFFLEN : bytes_to_recv;
		error = readplus( sock, buff, bytes_to_read );
		if ( ( error = fwrite( buff, bytes_to_read, 1, fp ) ) < 0 )
			break;
		memset( buff, 0, BUFFLEN );
		bytes_to_recv -= bytes_to_read;
	}

	if ( bytes_to_recv < 0 ) {
		DBG_ERR_MSG( "recv_file: apparently recieved %i more bytes than file specified", -bytes_to_recv );
		return -5;
	} else if ( error ) {
		DBG_ERR_MSG( "recv_file: socket failed to read with %i", error );
		return -1;
	} else if ( ( error = ferror( fp ) ) != 0 ) {
		DBG_ERR_MSG( "recv_file: failed to write file with %i", error );
		return -2;
	}

	fclose( fp );
	return 0;
}

struct clinfo {
	int fd;
	struct sockaddr_in6 addr;
};

// spawn a thread to act as a server
int init_server ( int (*function)( struct clinfo* ) ) {
	struct sockaddr_in6 saddr;
	struct sockaddr_storage caddr;
	struct clinfo* info = 0;
	unsigned int caddr_len = sizeof( struct sockaddr_storage );
	int sfd = 0;
	int cfd = 0;

	saddr.sin6_addr = IN6ADDR_ANY_INIT;
	saddr.sin6_port = htons( PORT );
	saddr.sin6_family = AF_INET6;

// 	TODO: make this TLS compliant
// 	TODO: TEST that errno will continue to be set on thread return
	if ( ( sfd = socket( AF_INET6, SOCK_STREAM, 0 ) ) == -1 ) return -1;
	if ( bind( sfd, (struct sockaddr*)&saddr, sizeof( struct sockaddr_in6 ) ) == -1 ) return -2;
	if ( listen( sfd, SOMAXCONN ) == -1 ) return -3;

	while ( 1 ) {
		int cfd = accept( sfd, (struct sockaddr*)&caddr, &caddr_len );
		if ( cfd == -1 ) {
			DBG_ERR_MSG( "init_server: accept failed" );
			close( cfd );
			continue;

// 		TODO: maybe make addresses more flexible? How to do this securely?
		} else if ( caddr_len != sizeof( struct sockaddr_in6 ) ) {
			DBG_ERR_MSG( "init_server: client didn't use IP6" );
// 			It specifically says in the manual that we should NOT try close again after failure, as this usually means the fd is closed already.
			if ( close( cfd ) == -1 ) DBG_ERR_MSG( "init_server: close failed" );
			continue;
		}

// 		copy info and call
		info = malloc( sizeof( struct clinfo ) );
		if ( info == 0 ) {
			DBG_ERR_MSG( "init_server: malloc failed, aborting" );
			abort();
		}
		info->fd = cfd;
		memcpy( &(info->addr), &caddr, sizeof( struct sockaddr_in6 ) );
		function( info );
		info = 0;

		caddr_len = sizeof( struct sockaddr_storage );
	}
}

// spawn a thread to act as a client
int init_client () {

}
