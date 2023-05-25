
#include "net-internal.h"

/*
 * hname: gets addresses using getaddrinfo, then calls the callback function on the FIRST of these.
 * If the callback returns below 0, it tries another until it either runs out or the callback returns above 0 (gives room for multiple failure conditions, with 0 being success).
 * 		- hostname: the hostname to find the addr of
 * 		- port: the port (also a string for a generic service)
 * 		- err_code: To get a more specific error message, you can provide an int* here.
 * 		- flags: any flags for getaddrinfo
 * 		- callback: takes a struct sockaddr_in6* addr and an optional argument. It then does something with the addr.
 * 		- oth: pointer to another argument to pass to callback, set to 0 if not needed.
 * RETURNS: -1 on getaddrinfo fail (errors are defined by EAI_{error name}) and -2 on callback fail.
 */
int hname ( const char* hostname,
			const char* port,
			int* err_code,
			int flags,
			int (*callback) ( const struct sockaddr* addr,
							  int addrlen,
							  void* oth ),
		    void* oth ) {

	assert( hostname != 0 );
	assert( port != 0 );
	assert( callback != 0 );

	struct addrinfo hints;
	struct addrinfo* result;
	struct addrinfo* index;

	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;		// can handle both v4 and v6
	hints.ai_flags = flags;				// leave flags up to caller
	hints.ai_socktype = SOCK_STREAM;	// we deal w tcp
	hints.ai_protocol = TCP_PROT;		// t c p

	int error_code = getaddrinfo( hostname, port, &hints, &result );
	if ( error_code ) {
// 		getaddrinfo failed; logging is up to caller
		if ( err_code ) *err_code = error_code;
		return -1;
	}

	index = result;
	error_code = -1;
	while ( index && error_code <= -1 ) {
// 		TODO: Only pass an IPV6 structure by converting an AF_INET struct to an AF_INET6 struct
		if ( index->ai_family == AF_INET || index->ai_family == AF_INET6 )
			error_code = callback( index->ai_addr, index->ai_addrlen, oth );
		index = index->ai_next;
	}

	freeaddrinfo( result );
	if ( error_code != 0 ) {
		if ( err_code ) *err_code = error_code;
		return -2;
	} else
		return 0;
}

/*
 * readplus
 */
int readplus ( int fd, char* buff, int len ) {
	assert( buff != 0 );

#ifndef TRANSIT_FUNCTIONS_DO_A_LOOP
	return read( fd, buff, len );
#else
	int tries = 128;
	int return_code = 0;
	int amt = 0;
	for ( int tries = 128; tries > 0 && amt >= len; tries-- ) {
		return_code = read( fd, buff + amt, len - amt );
		if ( return_code == -1 ) return -1;
		amt += return_code;
	}
	if ( tries < 0 ) return -2;
// 	returns how many MORE bytes were read
	if ( amt > len ) return len - amt - 2;
	return amt;
#endif
}

int writeplus ( int fd, const char* buff, int len ) {
	assert( buff != 0 );

#ifndef TRANSIT_FUNCTIONS_DO_A_LOOP
	return read( fd, buff, len );
#else
	int tries = 128;
	int return_code = 0;
	int amt = 0;
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

