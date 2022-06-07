
#include "main.h"

int main ( int argc, char** argv ) {
// 	We can only supply as many arguments as we have default values.
// 	Also, we can't use OPTSIZE cause multiple options can fix one argument.
	char* supplied[ DEFSIZE ] = { 0 };

// 	Simple arg parser
	if ( argc > 1 ) {
		for ( int i = 1; i < argc; i++ ) {
			int j = 0;
// 			Check if arg is equal to an option string
			for ( ; j < OPTSIZE; j++ ) {
				if ( !strcmp( argv[ i ], OPTIONS[ j ] ) )
					break;
			}
			assert( j < sizeof( OPTSIZE ) && j >= 0 );
// 			If it was, j holds the option. If not, j holds sizeof( OPTIONS ), and default catches this and all other cases.
			switch ( j ) {
				case 0:
				case 1:
					TAKE_ARG( 0, "-c [config file] OR --config [config file]" );
					break;
				default:
					fprintf( stderr, "Invalid arguments.\nType --help for more details.\n" );
					return 2;
			}
		}
	}

	for ( int i = 0; i < DEFSIZE; i++ )
		DBGLOG( "Argument %i", i, supplied[ i ] );
// 	Next, access the config file (supplied[ 0 ] or DEFAULTS[ 0 ] if no alternate provided)..
	FILE* config_file = 0;
	if ( supplied[ 0 ] == 0 ) {
		LOG( "No config given, opening default..." );
// 		Try to open the configuration
		config_file = fopen( DEFAULTS[ 0 ], "r+" );
// 		If not, create a new configuration and write default options, plus any user given options such as host info
		if ( config_file == 0 ) {
			if ( errno == ENOENT ) {
				LOG( "Default config doesn't exist, creating one..." );
				config_file = fopen( DEFAULTS[ 0 ], "w" );
				if ( config_file == 0 )
					ERR_QUIT( "Creating default config faile" );
			} else
				ERR_QUIT( "Opening default config failed" );
		}
// 		At this point, we have either opened config or erred and quit, so our job is finished.
	} else {
		LOG( "Opening config file '%s'", supplied[ 0 ] );
// 		Try to open the user supplied configuration.
		config_file = fopen( supplied[ 0 ], "r+" );
		if ( config_file == 0 )
			ERR_QUIT( "Opening file '%s' failed", supplied[ 0 ] );
	}

	fprintf( config_file, "%s\n", "hello" );

	fclose( config_file );

	return 0;
}
