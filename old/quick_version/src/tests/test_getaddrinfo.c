#include "net.h"

int callback ( const struct sockaddr* addr, int addrlen ) {
	char addrstr[ INET6_ADDRSTRLEN ];
	if( addrlen == sizeof( struct sockaddr_in ) && inet_ntop( AF_INET, &(((struct sockaddr_in*)addr)->sin_addr), addrstr, INET_ADDRSTRLEN ) == 0 )
		ERR_QUIT( "inet_ntop had an error" );
	else if( addrlen == sizeof( struct sockaddr_in6 ) && inet_ntop( AF_INET6, &(((struct sockaddr_in6*)addr)->sin6_addr), addrstr, INET6_ADDRSTRLEN ) == 0 )
		ERR_QUIT( "inet_ntop had an error" );
	else
		ASSERT( "false" );

	printf( "addr: %s\n", addrstr );
	if( addrlen == sizeof( struct sockaddr_in6 ) )
		printf( "port: %i\n", ntohs( ((struct sockaddr_in6*)addr)->sin6_port ) );
	else
		printf( "port: %i\n", ntohs( ((struct sockaddr_in*)addr)->sin_port ) );
	return 0;
}

int main ( int argc, char *argv[] ) {
	char* node = argv[ 1 ];
	char* service = argv[ 2 ];//"443";
	int addr_error;

	if ( hname( node, service, &addr_error, 0, callback ) == -1 )
		ERR_QUIT( "hname failed, error code: %i", addr_error );

	return 0;
}
