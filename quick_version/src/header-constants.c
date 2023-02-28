

#include "arg-parser.h"
const char* OPTIONS[] = {
	"-c",
	"--config"
};
const int OPTSIZE = ( sizeof( OPTIONS ) / sizeof( *OPTIONS ) );
const char* DEFAULTS[] = {
	"./config"
};
const int DEFSIZE = ( sizeof( DEFAULTS ) / sizeof( *DEFAULTS ) );


#include "config.h"
const char* DEFAULT_CONFIG = "hewwo\n";


#include "net.h"
