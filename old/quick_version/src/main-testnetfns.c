
#include "net.h"
#define BUFFER_SIZE 6144

int bind_serv ( const struct sockaddr* addr, int addrlen, void* sk_pt ) {
	int sock = *((int*)sk_pt);
	if ( bind( sock, addr, addrlen ) != 0 )
		return errno;
	return 0;
}

int conn_cli ( const struct sockaddr* addr, int addrlen, void* sk_pt ) {
	int sock = *((int*)sk_pt);
	if ( connect( sock, addr, addrlen ) != 0 )
		return errno;
	return 0;
}

// Main thread that will listen for client requests and respond to them.
// clients always initiate connections.
int cl_listen ( struct sockaddr_in6* client_addr, int comm_socket ) {
	int e_c = 0;
	while ( ( e_c = write( comm_socket, "hewwo", 6 ) ) < 6 ) {
		if ( e_c == -1 )
			ERR_QUIT( "write failed" );
	}
	LOG( "sent hewwo to client" );
	return 0;
}

// comm thread for client
int se_listen ( struct sockaddr_in6* server_addr, int comm_socket ) {
	int e_c = 0;
	char buffer[ BUFFER_SIZE ];
	while ( ( e_c = read( comm_socket, buffer, 6 ) ) < 6 ) {
		if ( e_c == -1 )
			ERR_QUIT( "write failed" );
	}
	LOG( "server sent %s", buffer );
	return 0;
}

int main ( int argc, char* argv[] ) {
	int err_code = 0;

// 	dunno if this is necessary
	if ( net_init() == -1 )
		ERR_QUIT( "net init failed" );

	if ( argc == 2 ) {
// 		act like a server
		struct sockaddr_in6 cl_ad;
		unsigned int cl_ad_size = sizeof( struct sockaddr_in6 );
		int server_socket = socket( AF_INET6, SOCK_STREAM, 0 );
		int client_socket = -1;

		if ( server_socket == 0 )
			ERR_QUIT( "listen socket initialization failed" );

// 		this binds the server to an address given with AI_PASSIVE
		switch ( hname( 0, "8888", &err_code, AI_PASSIVE, bind_serv, &server_socket ) ) {
			case -1:
				ERR_QUIT( "getaddrinfo failed: %s", gai_strerror( err_code ) );
			case -2:
				errno = err_code;
				ERR_QUIT( "binding failed" );
		}

		if ( listen( server_socket, 1 ) != 0 )
			ERR_QUIT( "listen failed" );

		client_socket = accept( server_socket, (struct sockaddr*)&cl_ad, (unsigned int* restrict)&cl_ad_size );
		if ( client_socket == -1 )
			ERR_QUIT( "accept failed" );
		else if ( cl_ad_size != sizeof( struct sockaddr_in6 ) )
			ERR_QUIT( "client addr not ipv6" );

		struct sockaddr_in6* client_pass = malloc( sizeof( struct sockaddr_in6 ) );
		if ( client_pass == 0 )
			ERR_QUIT( "you're out of memory--should get that fixed kinda quick heh" );
		memcpy( client_pass, &cl_ad, sizeof( struct sockaddr_in6 ) );

// 		in the code I should make this run with the peer structure instead of raw client address.
// 		I would also authenticate the peer first.
// 		But, we're assuming I've already done that.
		cl_listen( client_pass, client_socket );
	} else {
// 		act like a client
		struct sockaddr_in6 se_ad;
		int conn_socket = socket( AF_INET6, SOCK_STREAM, 0 );

		if ( conn_socket == 0 )
			ERR_QUIT( "listen socket initialization failed" );

// 		gets the address of and connects to the server in one go
		switch ( hname( "::1", "8888", &err_code, AI_PASSIVE, conn_cli, &conn_socket ) ) {
			case -1:
				ERR_QUIT( "getaddrinfo failed: %s", gai_strerror( err_code ) );
			case -2:
				errno = err_code;
				ERR_QUIT( "connecting to peer failed" );
		}

// 		at this point we are connected, and we are skipping any sort of verification, so now
		se_listen( &se_ad, conn_socket );
	}
}
