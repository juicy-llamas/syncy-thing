
#ifndef THE_COMPLIANT_LOGGING_HEAD
#define THE_COMPLIANT_LOGGING_HEAD

// logging.h is the ONLY header that does NOT include root.h
// thus, it includes string itself
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void log_abort ( char* fmt, ... );
void log_err ( char* fmt, ...  );
void log_warn( char* fmt, ...  );
void log_note ( char* fmt, ...  );

#endif
