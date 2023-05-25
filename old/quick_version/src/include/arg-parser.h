
// This file holds things that are used by the argument parser
#ifndef __PARSER_HEAD
#define __PARSER_HEAD

// This is a list of all of the arguments that could be given to the program.
extern const char* OPTIONS[];
extern const int OPTSIZE;
// These are the default values for those arguments.
extern const char* DEFAULTS[];
extern const int DEFSIZE;

// A macro to help eliminate redundancy w/ the code in argument setting (making a fn take a *i seemed kinda weird, plus return wouldn't work).
#define TAKE_ARG( in, msg ) ({					\
	i += 1;										\
	if ( i < argc )								\
		supplied[ in ] = argv[ i ];				\
	else {										\
		fprintf( stderr, "Usage: " msg "\n" );	\
		return 2;								\
	}											\
})

#endif
