
#include "net.h"

int TCP_PROT;

int net_init () {
	int ret = 0;

	struct protoent* str = getprotobyname( "TCP" );
	if ( str )
		TCP_PROT = str->p_proto;
	else {
		WARN( "getprotobyname failed, defaulting to 6.\n\tIf you know value for TCP, or want to use a different proto, specify NET_PROTO=x in config." );
		TCP_PROT = 6;
		ret = 1;
	}

	return ret;
}

int hname ( const char* hostname,
			const char* port,
			int* err_code,
			int flags,
			int (*callback) ( const struct sockaddr* addr,
							  int addrlen,
							  void* oth ),
		    void* oth ) {
	struct addrinfo hints;
	struct addrinfo* result;
	struct addrinfo* index;

	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET6;			// can handle both v4 and v6
	hints.ai_flags = flags;				// leave flags up to caller
	hints.ai_socktype = SOCK_STREAM;	// we deal w tcp
	hints.ai_protocol = TCP_PROT;		// t c p

	int error_code = getaddrinfo( hostname, port, &hints, &result );
	if ( error_code )
		*err_code = error_code;
	if ( error_code != 0 )
		return -1;

	index = result;
	error_code = -1;
	while ( index && error_code <= -1 ) {
		if ( index->ai_family == AF_INET || index->ai_family == AF_INET6 )
			error_code = callback( index->ai_addr, index->ai_addrlen, oth );
		index = index->ai_next;
	}

	freeaddrinfo( result );
	if ( error_code != 0 ) {
		*err_code = error_code;
		return -2;
	} else
		return 0;
}
