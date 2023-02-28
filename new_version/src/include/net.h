
// header file for network files

#ifndef __NET_HEAD
#define __NET_HEAD

#include "root.h"

// Most socket stuff
#include <sys/types.h>
#include <sys/socket.h>
// sockaddr_in/6
#include <netinet/in.h>
// inet_ntop
#include <arpa/inet.h>
// getaddrinfo and protocol stuff
#include <netdb.h>
// read & write
#include <unistd.h>

extern int TCP_PROT;

// implementations found in net-help.c

// init for network specific stuff.
int net_init ();

// runs the callback on the addresses outputed by getaddrinfo. Returns 0 on success, -1 if callback failed, and -2 if getaddrinfo failed.
// callback / getaddrinfo error codes are put in err_code
// addrinfo_err can be 0, hostname and port correspond with node and service in getaddrinfo,
int hname ( const char* hostname,
			const char* port,
			int* err_code,
			int flags,
			int (*callback) ( const struct sockaddr* addr,
							  int addrlen,
							  void* oth ),
		    void* oth );

#endif
