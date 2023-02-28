
#include "main.h"

int main ( int argc, char** argv ) {
// 	We can only supply as many arguments as we have default values.
	const char* supplied[ DEFSIZE ];

// 	Set defaults
	for ( int i = 0; i < DEFSIZE; i++ )
		supplied[ i ] = DEFAULTS[ i ];

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
					fprintf( stderr, "invalid arguments.\ntype --help for more details.\n" );
					return 2;
			}
		}
	}

	for ( int i = 0; i < DEFSIZE; i++ )
		DBGLOG( "argument %i: %s", i, supplied[ i ] );

	set_config( supplied );

	return 0;
}
