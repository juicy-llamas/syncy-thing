
#include "net-internal.h"

struct ip_list {
	struct ip_list* next;
	int type;
	union {
		int ipv4;
		long ipv6[ 2 ];
	} ip;
};

void print_ip ( int family, void* addy ) {
	assert( addy != 0 );

	if ( family == AF_INET ) {
		struct sockaddr_in* addyv4 = (struct sockaddr_in*)addy;
		printf( "struct sockaddr_in\n" );
		printf( "\tsin_family: %i\n", addyv4->sin_family );
		printf( "\tsin_port (little endian): %i\n", htons( addyv4->sin_port ) );
		printf( "\tsin_addr: %i.%i.%i.%i\n", ((char*)&(addyv4->sin_addr.s_addr))[ 0 ],
											 ((char*)&(addyv4->sin_addr.s_addr))[ 1 ],
											 ((char*)&(addyv4->sin_addr.s_addr))[ 2 ],
											 ((char*)&(addyv4->sin_addr.s_addr))[ 3 ] );
	} else if ( family == AF_INET6 ) {
		struct sockaddr_in6* addyv6 = (struct sockaddr_in6*)addy;
		printf( "struct sockaddr_in6\n" );
		printf( "\tsin6_family: %i\n", addyv6->sin6_family );
		printf( "\tsin6_port (little endian): %i\n", htons( addyv6->sin6_port ) );
		printf( "\tsin6_flowinfo: %i\n", htons( addyv6->sin6_flowinfo ) );
		printf( "\tsin6_addr: %i:%i:%i:%i:%i:%i:%i:%i\n",
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 0 ],
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 1 ],
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 2 ],
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 3 ],
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 4 ],
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 5 ],
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 6 ],
										   ((short*)&(addyv6->sin6_addr.s6_addr))[ 7 ] );
		printf( "\tsin6_scope_id: %i\n", htons( addyv6->sin6_scope_id ) );
	} else {
		log_warn( "tried to print IP, but family was invalid" );
	}
}

int hname_callback ( struct sockaddr* addy, int addylen, void* oth ) {
	UNUSED( oth );
	if ( addylen == sizeof( struct sockaddr_in ) ) {

	} else if ( addylen == sizeof( struct sockaddr_in6 ) ) {

	} else {
		return -1;
	}
}

struct ip_list* get_ip_of ( char* name ) {
	char cmd[ 2048 ];
	char* out = malloc( 2048 );
	int outsize = 2048;
	int error = 0;
	FILE* cmdout = 0;
	struct ip_list* ret = malloc( sizeof( ip_list ) );
	struct ip_list* cur = ret;
	int addr[ 4 ];

// 	since this is test code, not production code, I don't care, but this is unsafe and should be subbed with 'snprintf' or something.
	sprintf( cmd, "host \"%s\" | awk '{ if (/has address/) { print $4 } else { print $5 } }'", name );
	cmdout = popen( cmd, "r" );

	while ( ( error = getline( &out, &outsize, cmdout ) ) != 0 ) {
		if ( error == -1 )
			log_abort( "failed to get awk command output" );

		if ( inet_pton( AF_INET, cmdout, addr ) == 1 ) {
			cur->type = AF_INET;
			cur-.ip.ipv4 = addr[ 0 ];
		} else if ( inet_pton( AF_INET6, cmdout, addr ) == 1 ) {
			cur->type = AF_INET6;
			cur->ip.ipv6[ 0 ] = ((long*)addr)[ 0 ];
			cur->ip.ipv6[ 1 ] = ((long*)addr)[ 1 ];;
		} else {
			warn( "invalid addr: '%s'", cmdout );
			if ( cur == ret ) {
				ret = 0;
			}
			free( cur );
			return ret;
		}
	}

	free( out );
	return ret;
}

int main () {

// 	get ip of various hosts
	struct ip_list* google = get_ip_of( "www.google.com" );
	for ( struct ip_list* ptr = google; ptr != 0; ptr = ptr->next )


// 	test hname
	assert( hname( "www.google.com

// 	init two sockets

	struct sockaddr_in6 serv = {0};
	struct sockaddr_in6 clnt = {0};

	serv.sin6_family = AF_INET6;
	serv.sin6_port = htons( 8888 );
	serv.sin6_addr = IP6ADDR_ANY_INIT;

	clnt.sin6_family = AF_INET6;
	clnt.sin6_port = htons( 8888 );
	clnt.sin6_addr = IP6ADDR_LOOPBACK_INIT;

	int sock1 = socket( AF_INET6, SOCK_STREAM, 0 );
	int sock2 = socket( AF_INET6, SOCK_STREAM, 0 );
	if ( sock1 == -1 || sock2 == -1 )
		log_abort( "socket creation failed" );

	if ( bind( sock1

	readplus();
}
