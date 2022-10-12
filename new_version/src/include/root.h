
#ifndef __ROOT_HEAD
#define __ROOT_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "options.h"

// Helper macros.

#define ERR_QUIT( fmt, ... ) ({										\
	char* estr = strerror( errno );									\
	fprintf( stderr, "FATAL ERROR (file %s, line %i): " 			\
			fmt ": %s\n", __FILE__, __LINE__, 						\
			##__VA_ARGS__, estr );									\
	abort();														\
})

#define ERR( fmt, ... ) ({											\
	char* estr = strerror( errno );									\
	fprintf( stderr, "ERROR (file %s, line %i): " 					\
			fmt ": %s\n", __FILE__, __LINE__, 						\
			##__VA_ARGS__, estr );									\
})

#define WARN( fmt, ... ) 											\
	printf( "WARNING (file %s, line %i): " fmt "\n",				\
			__FILE__, __LINE__, ##__VA_ARGS__ );

#ifdef __LOG_TO_STD
	#define LOG( fmt, ... ) 										\
	printf( "LOG (file %s, line %i): " fmt "\n",					\
			__FILE__, __LINE__, ##__VA_ARGS__ );
#else
	#define LOG( ... ) ((void)0)
#endif

#ifdef __DEBUG
	#define DBGLOG( fmt, ... )  									\
	printf( "DEBUG (file %s, line %i): " fmt "\n",					\
			__FILE__, __LINE__, ##__VA_ARGS__ );
#else
	#define DBGLOG( ... ) ((void)0)
#endif

#endif
