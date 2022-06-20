
#include "config.h"

int set_config ( char* supplied[ DEFSIZE ] ) {
// 	Next, access the config file (supplied[ 0 ] or DEFAULTS[ 0 ] if no alternate provided)..
	FILE* config_file = 0;
	LOG( "Opening config file '%s'", supplied[ 0 ] );
// 		Try to open the user supplied configuration.
	config_file = fopen( supplied[ 0 ], "r+" );
	if ( config_file == 0 ) {
		if (
		ERR_QUIT( "Opening file '%s' failed", supplied[ 0 ] );
	}

	fprintf( config_file, "%s\n", "hello" );

	fclose( config_file );
}
