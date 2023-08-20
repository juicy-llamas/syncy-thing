
#include "root.h"
#include <sys/inotify.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>

// #define CHECK_IF_FAIL( stuff, failcond ) ({			\
// 	if ( ( stuff ) failcond )
// 		log_err( "stuff failed!" );

/*
 * Test goal: to implement some basic inotify calls, prototype a theoretical
 * class that can be used in a multi-platform environment, and make it somewhat
 * functional (for linux using inotify).
 */

/*
 * Unix inotify impl
 * This is for unix systems, or really systems that support inotify
 * (which is practically all linux systems). The notify_instance
 * structure is a bridge among these and may support certain common
 * fields, but maybe not :/
 */

struct monitor {
// 	inotify specific
	int inotify_inst;
	int* inotify_desc_list;

// 	general
	char** file_list; // ???
	unsigned int file_amt;
	unsigned int file_len;
};

// Assumes thing has been malloced
int monitor_init ( struct monitor* thing ) {
	thing->file_amt = 0;
	thing->file_len = 16;
	thing->file_list = malloc( thing->file_len * sizeof( char* ) );
	if ( thing->file_list == 0 )
		log_abort( "malloc failed--aborting" );
// 	the descriptor list uses the same amt and size parameters as the file_len
// 	as they're adjusted together.
	thing->inotify_desc_list = malloc( thing->file_len * sizeof( int ) );
	if ( thing->inotify_desc_list == 0 )
		log_abort( "malloc failed--aborting" );

	thing->inotify_inst = inotify_init();
	if ( thing->inotify_inst == -1 ) {
		log_err( "inotify instance failed to open" );
		return -1;
	}

	return 0;
}

// There must be at least one file specified. Len is the number of pointers.
int monitor_add ( struct monitor* inst, const char* const* file_lst, size_t len ) {
	assert( file_lst );
	assert( *file_lst );
	int watch_desc = 0;

	if ( ( ( inst->file_len >> 2 ) + ( inst->file_len >> 1 ) ) < ( inst->file_amt + len ) ) {
		inst->file_len <<= 1;
		inst->file_list = realloc( inst->file_list, inst->file_len );
		if ( inst->file_list == 0 )
			log_abort( "realloc failed" );
	}

	for ( size_t i = 0; i < len; i++ ) {
		watch_desc = inotify_add_watch( inst->inotify_inst, file_lst[ i ], IN_ATTRIB | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE );
		if ( watch_desc == -1 ) {
			log_err( "adding path (%s) to inotify watchlist failed", file_lst[ i ] );
			return -1;
		}

		log_debug( "watch_desc: %i", watch_desc );

		inst->inotify_desc_list[ inst->file_amt ] = watch_desc;
		inst->file_list[ inst->file_amt ] = strdup( file_lst[ i ] );
		inst->file_amt += 1;
	}

	return 0;
}

void* monitor_poll ( void* the_stuff_is_real ) {
	struct monitor* inst = (struct monitor*) the_stuff_is_real;
	int max_size = sizeof( struct inotify_event ) + NAME_MAX + 1;
	char buff[ max_size ];
	memset( buff, 0, max_size );
// 	so I don't have to cast the buff every time
	struct inotify_event* event = (struct inotify_event*)buff;

	int i = 3;

// 	infinite loop designed for a thread--in reality you can do this any way you wish.
	while ( i ) {
// 		get the event
		log_debug( "inotify desc: %i", inst->inotify_inst );
		int len = read( inst->inotify_inst, event, max_size );
		if ( len == -1 ) {
			log_err( "inotify read 1 failed" );
// 			return -1;
		}
// 		dispatch the event to the correct thread (TODO)
// 		for now just print
		printf( "INOTIFY_EVENT: 0x%08X | 0x%08X | 0x%08X | 0x%08X | %s\n", event->wd, event->mask, event->cookie, event->len, event->name );

		i--;
	}

	return 0;
}

// frees stuff. Does not free the structure though
void monitor_free ( struct monitor* inst ) {
	free( inst->inotify_desc_list );
	for ( unsigned int i = 0; i < inst->file_amt; i++ )
		free( inst->file_list[ i ] );
	free( inst->file_list );
}

int main ( int argc, char** argv ) {
	const char* f1 = "file_one";
	const char* f2 = "file_two";
	const char* f3 = "file_three";
	const char* d1 = "directory_one";
	const char* d2 = "directory_two";
	const char* f1d1 = "directory_one/file_one";
	const char* f2d1 = "directory_one/file_two_different_name";
	const char* f3d1 = "directory_one/file_three";
	const char* f1d2 = "directory_two/file_one";
	const char* f2d2 = "directory_two/file_two_different_name";
	const char* f3d2 = "directory_two/file_three_another_different_name";

	const char* const list[] = { f1, f2, f3, d1, d2 };

	FILE* pf1 = fopen( f1, "w+" );
	FILE* pf2 = fopen( f2, "w+" );
	FILE* pf3 = fopen( f3, "w+" );
	mkdir( d1, 0xFFFF );
	mkdir( d2, 0xFFFF );
	FILE* pf1d1 = fopen( f1d1, "w+" );
	FILE* pf2d1 = fopen( f2d1, "w+" );
	FILE* pf1d2 = fopen( f1d2, "w+" );

	struct monitor m;
	monitor_init( &m );
	monitor_add( &m, list, 5 );

// 	return 1;

	pthread_t thr;
	pthread_create( &thr, 0, monitor_poll, &m );

	struct timespec waittime = { 0, 200 * 1000000 };

	nanosleep( &waittime, 0 );
	printf( "executing %s\n", f3d1 );
	FILE* pf3d1 = fopen( f1d1, "w+" );

	nanosleep( &waittime, 0 );
	printf( "executing %s\n", f2d2 );
	FILE* pf2d2 = fopen( f2d1, "w+" );

	nanosleep( &waittime, 0 );
	printf( "executing %s\n", f3d2 );
	FILE* pf3d2 = fopen( f1d2, "w+" );


	nanosleep( &waittime, 0 );
	nanosleep( &waittime, 0 );

	pthread_kill( thr, 9 );
	pthread_join( thr, 0 );

	fclose( pf1 );
	fclose( pf2 );
	fclose( pf3 );
	fclose( pf1d1 );
	fclose( pf2d1 );
	fclose( pf3d1 );
	fclose( pf1d2 );
	fclose( pf2d2 );
	fclose( pf3d2 );

	unlink( f1 );
	unlink( f2 );
	unlink( f3 );
	unlink( f1d1 );
	unlink( f2d1 );
	unlink( f3d1 );
	unlink( f1d2 );
	unlink( f2d2 );
	unlink( f3d2 );
	rmdir( d1 );
	rmdir( d2 );

	return 0;
}
