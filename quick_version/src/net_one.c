
#include "net.h"

#define LISTEN_PORT 666
#define BACKLOG 1

typedef struct {
	const char* name;
	int buffer_len
} sync_node;

typedef struct {
	char* hostname;
	int hostlen;
	struct sockaddr_in addr;
	int comm_sock;
	pthread_t thread;
} sync_peer;

typedef struct {
	short listen_port;		// port to listen on
	sync_node** node_list;	// known nodes
	sync_peer** peer_list;	// known peers
	int node_len;
	int peer_len;
	int buffer_len;			// how long comm buff should be (longer buffers are more efficient for bigger files, but not smaller ones)
	char** map;				// if file i is shared with client j, we put a 1 in the map, else put a 0. We can also include other flags here.
} config;

typedef struct {
	config* conf;
	sync_peer* peer;
} pfn_args;

// int connect_to_peer ( sync_peer* peer ) {
// 	if ( conn_socket != 0 )
// 		return 0;
// 	else {
//
// 	}
// }

int listen_for_peers ( config* conf ) {
	int listen_sock = socket( AF_INET, SOCK_STREAM, 0 );
	int accept_sock = -1;
	struct sockaddr_in this_addr;
	struct sockaddr_in peer_addr;
	int peer_addr_size = sizeof( peer_addr );

// 	Standard procedure for starting a socket server
	if ( listen_sock == -1 ) ERR_QUIT( "Socket creation failed" );

	this_addr.sin_port = htons( conf->listen_port );
	this_addr.sin_family = AF_INET;
	this_addr.sin_addr.s_addr = 0;

	if ( bind( listen_sock, (struct sockaddr*)&this_addr, sizeof( this_addr ) ) != 0 )
		ERR_QUIT( "Binding socket to addr failed (is port %i reserved?)", conf->listen_port );

	if ( listen( listen_sock, BACKLOG ) != 0 )
		ERR_QUIT( "Setting socket to listen failed" );

	LOG( "Listening on port %i...", conf->listen_port );

	accept_sock = accept( listen_sock, (struct sockaddr *)&peer_addr, &peer_addr_size );
	while ( accept_sock != -1 ) {
// 		We check if the peer address matches a peer in our config file
		LOG( "Got a connection" );
		sync_peer* this_peer = 0;
		for ( int i = 0; i < conf->peer_len; i++ ) {
			if ( peer_addr.sin_addr.s_addr == conf->peer_list[ i ]->addr.sin_addr.s_addr ) {
				this_peer = conf->peer_list[ i ];
				LOG( "Connection confirmed to be the address of peer %s", conf->peer_list[ i ]->hostname );
				break;
			}
		}

		if ( this_peer != 0 ) {
// 			We then verify the hostname
			char comm_buffer[ conf->buffer_len ];	// MAKE SURE YOU VALIDATE BUFFER_LEN
			memset( comm_buffer, 0, conf->buffer_len );
			int tries = 3;
			int false_peer = 0;

			LOG( "Reading Hostname..." );
			for ( int i = 0; i < this_peer->hostlen && tries > 0; i += 1024 ) {
				int ret_code = read( accept_sock, comm_buffer, conf->buffer_len );
				if ( ret_code == 0 ) {
					DBGLOG( "ret code 0" );
					i -= 1024;
					tries -= 1;
				} else if ( ret_code == -1 ) {
					DBGLOG( "ret code -1, errno is %s", strerror( errno ) );
					i -= 1024;
					tries -= 1;
				} else {
					if ( strncmp( this_peer->hostname, comm_buffer, 1023 ) ) {
						false_peer = 1;
						break;
					}
				}
			}

			if ( !false_peer ) {
				LOG( "Peer is accepted, peer thread starting..." );
				peer_thread_fn( (void*)this_peer );
			} else
				LOG( "WARNING: Received hostname did not match hostname on file, peer was rejected.\n\tPeer IP: %s\n", inet_ntoa( peer_addr.sin_addr ) );
		}
		peer_addr_size = sizeof( peer_addr );
		accept_sock = accept( listen_sock, (struct sockaddr*)&peer_addr, &peer_addr_size );
	}
	LOG( "Server stopped accepting peers.\n\tReason: %s", strerror( errno ) );
}

int peer_thread_fn ( void* arg ) {
	sync_peer* this_peer = (sync_peer*)arg;

}

int main () {

}
