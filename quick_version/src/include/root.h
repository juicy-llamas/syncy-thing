
#ifndef __ROOT_HEAD
#define __ROOT_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "options.h"

// More helper macros.
#define ERR_QUIT( fmt, ... ) ({															\
	char* estr = strerror( errno );														\
	fprintf( stderr, "ERROR (line %i): " fmt ": %s\n", __LINE__, ##__VA_ARGS__, estr );	\
	return 1;																			\
})

#ifdef __LOG_TO_STD
	#define LOG( fmt, ... ) printf( "LOG (line %i): " fmt ".\n", __LINE__, ##__VA_ARGS__ )
#else
	#define LOG( ... ) ((void)0)
#endif

#ifdef __DEBUG
	#define DBGLOG( fmt, ... ) printf( "DEBUG (line %i): " fmt ".\n", __LINE__, ##__VA_ARGS__ )
#else
	#define DBGLOG( ... ) ((void)0)
#endif

#endif
