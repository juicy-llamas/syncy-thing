
#ifndef OMG_LOOK_HOW_COMPLIANT_IT_IS_NET_INTERNAL_H
#define OMG_LOOK_HOW_COMPLIANT_IT_IS_NET_INTERNAL_H

#include "root.h"

/*
 * Defs
 */

// standard buffer length
#define BUFFLEN 4096
#define TRANSIT_FUNCTIONS_DO_A_LOOP
extern int TCP_PROT;

/*
 * Includes from stdlib
 */

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

// internal helper fns
int hname ( const char* hostname,
			const char* port,
			int* err_code,
			int flags,
			int (*callback) ( const struct sockaddr* addr,
							  int addrlen,
							  void* oth ),
		    void* oth );
int readplus ( int fd, char* buff, int len );
int writeplus ( int fd, const char* buff, int len );

#endif
