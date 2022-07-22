
#include "config.h"

int set_config ( const char** supplied ) {
	char* buffer = malloc( 1024 );
	if ( buffer == 0 ) ERR_QUIT( "mem" );
	int buffer_size = 1024;
// 	Access the config file (supplied[ 0 ] or DEFAULTS[ 0 ] if no alternate provided)..
	FILE* config_file = 0;
	LOG( "opening config file '%s'", supplied[ 0 ] );
	config_file = fopen( supplied[ 0 ], "r+" );

	if ( config_file == 0 ) {
		WARN( "config not found, creating one" );

		config_file = fopen( supplied[ 0 ], "w" );
		if ( config_file == 0 )
			ERR_QUIT( "failed to open config for writing" );

		fprintf( config_file, "%s\n", DEFAULT_CONFIG );
	}

// 	We then parse the config file
	int parsing_mode = 0; // what are we parsing rn?
	while ( getline( &buffer, &buffer_size, config_file ) != -1 ) {
		if ( buffer[ 0 ] != '#' ) {
			switch ( parsing_mode ) {
				case 0:
					if ( buffer[ 0 ] == '.' ) {
// 						my attempt at 'branchless programming'.
// 						also yes it fails if buffer holds stuff >6. do i care? no. strncmp is safer
						parsing_mode =
							1 * !strncmp( buffer+1, "hosts", 5 ) +
							2 * !strncmp( buffer+1, "files", 5 ) +
							3 * !strncmp( buffer+1, "blacklist", 9 ) +
							4 * !strncmp( buffer+1, "lan-list", 8 );
					}
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
			}
		}
	}
	if ( errno == ENOMEM ) ERR_QUIT( "mem" );

	free( buffer );
	fclose( config_file );
	return 0;
}
