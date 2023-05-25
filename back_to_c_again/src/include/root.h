 
#ifndef I_AM_GROOT_HEAD__xXEXTRA_DISTINCTIVE_VERSIONXx
#define I_AM_GROOT_HEAD__xXEXTRA_DISTINCTIVE_VERSIONXx

/*
 * IMPORTANT Definitions.
 */

// disables asserts. Up to the builder.
// #define NDEBUG

// uses the logging macros instead of the logging functions defined in logging.h,
// which write to both stdout/err and a log file (tbd). Should be disabled once
// the logging macros are sufficiently tested and safe. You cannot use the logger
// functions and debug at the same time.
#define USE_LOGGING_MACROS

/*
 * Basic C stdlib includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

/*
 * Helper logging macros
 */

#ifdef USE_LOGGING_MACROS

	#define log_abort( fmt, ... ) ({									\
		char* estr = strerror( errno );									\
		fprintf( stderr, "FATAL ERROR (file %s, line %i): " 			\
				fmt ": %s\n", __FILE__, __LINE__, 						\
				##__VA_ARGS__, estr );									\
		abort();														\
	})

	#define log_err( fmt, ... ) ({										\
		char* estr = strerror( errno );									\
		fprintf( stderr, "ERROR (file %s, line %i): " 					\
				fmt ": %s\n", __FILE__, __LINE__, 						\
				##__VA_ARGS__, estr );									\
	})

	#define log_warn( fmt, ... ) 						 				\
		printf( "WARNING (file %s, line %i): " fmt "\n",				\
				__FILE__, __LINE__, ##__VA_ARGS__ );

	#define log_note( fmt, ... ) 										\
		printf( "LOG (file %s, line %i): " fmt "\n",					\
				__FILE__, __LINE__, ##__VA_ARGS__ );

	#ifndef NDEBUG
		#define log_dbgnote( fmt, ... )  								\
			printf( "DEBUG (file %s, line %i): " fmt "\n",				\
					__FILE__, __LINE__, ##__VA_ARGS__ );

// 		vec is a void* array (any will do) and
		#define printvec( vec, size ) ({								\
			printf( "PRINTING VECTOR \"%s\" (file %s, line %i): \n[ ",	\
					#vec, __FILE__, __LINE__ );							\
			for ( int i = 0; i < size; i++ ) {							\
				printf( "0x%02X, ", ((char*)vec)[ i ] );				\
				if ( i != 0 && i % 16 == 0 )							\
					printf( "\n" );										\
			}															\
			printf( "]" );												\
		})
	#else
		#define log_dbg( fmt, ... ) ((void)0)
		#define printvec( vec, size) ((void)0)
	#endif

#else
	#include "logging.h"
	#define log_dbgnote( fmt, ... ) ((void)0)
	#define printvec( vec, size ) ((void)0)
#endif

/*
 * Other useful macros, such as suppressing the unused warning.
 */

#define UNUSED( a ) ( (void)( 0 + a ) )

#endif
