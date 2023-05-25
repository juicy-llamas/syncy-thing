
#include "main.h"
#include "net.h"

int conn_stub ( const struct sockaddr* addr, int addrlen, void* oth ) {
	/*current peer*/ = (/*peer datatype*/ *)oth;
	/*assign the address struct to the entry*/
	if ( connect( /*peer socket*/, addr, addrlen ) != 0 )
		return errno;
	return 0;
}

int bind_stub ( const struct sockaddr* addr, int addrlen, void* oth ) {
	int sock = *((int*)oth);
	if ( bind( sock, addr, addrlen ) != 0 )
		return errno;
	return 0;
}

int check_for_peers ( /*list of peers*/ ) {
	for ( /*every peer in list*/ ) {
		int error = 0;
		int /*peer socket*/ = socket( AF_INET6, SOCK_STREAM, 0 );

		if ( /*peer socket*/ == -1 )
			return 1; // ERR_QUIT( "failed to initiate client_fd for peer \"%s\"", /*peer hname*/ );

		switch ( hname( /*peer hname*/, /*peer port*/, &error, 0, conn_stub, /*current peer*/ ) ) {
			case -1:
				ERR( "failed to resolve peer hostname \"%s\", skipping", /*peer hname*/ );
				continue;
			case -2:
				ERR( "failed to connect to peer \"%s\", skipping", /*peer hname*/ );
				continue;
		}

		/*verify peer*/
		/*launch a peer listen thread*/
	}
	return 0;
}

int accept_loop ( int listen_fd, /*list of peers*/ ) {
	int accept_fd = 0;
	struct sockaddr *accept_addr = 0;
	int accept_addr_len = 0;

	while ( ( accept_fd = accept( listen_fd, &accept_addr, &accept_addr_len ) ) != -1 && /*there are still peers in our predetermined list*/ ) {
		switch ( /*validate peer*/ ) {
			/*various error cases can print various errors on the screen*/
			case 0:
				/*launch a peer listen thread*/
				break;
		}
	}
}

int start_server ( /*list of peers*/ ) {
	int error = 0;
	int listen_fd = socket( AF_INET6, SOCK_STREAM, 0 );

	if ( listen_fd == -1 )
		return 1; // ERR_QUIT( "failed to initiate listen_fd for server", /*current peer name*/ );

	switch ( hname( "::1", /*port*/, &error, 0, bind_stub, &listen_fd ) ) {
		case -1:
			return 2; // ERR_QUIT( "failed to resolve server address" );
		case -2:
			return 3; // ERR_QUIT( "failed to bind server", );
	}

	if ( listen( listen_fd, 6 ) == -1 )
		return 4; // ERR_QUIT( "failed to listen to socket" );

	/*start accept_loop as pthread*/

	return 0;
}

int main ( int argc, char** argv ) {
// 	parse args

	switch ( check_for_peers ) {

	}


	return 0;
}
