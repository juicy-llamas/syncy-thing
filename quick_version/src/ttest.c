#include "net.h"

int main () {
	struct sockaddr_in k;
	struct sockaddr_in6 k2;
	printf( "%lx, %lx, %lx, %lx\n", &k, &(k.sin_family), &k2, &(k2.sin6_family) );
	this is incorrect
}
