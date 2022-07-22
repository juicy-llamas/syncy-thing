
#ifndef __CONFIG_HEAD
#define __CONFIG_HEAD

#include "main.h"

extern const char* DEFAULT_CONFIG;

typedef struct {
	char* name;
	struct sockaddr_in6 addr;
	int sock;

	sync_file** files;
	char* file_perms;
	int num_files;
} sync_peer;

typedef struct {
	char* name;
	long int size;
	int inot_w_fd;
	int inot_i_fd;
} sync_file;

#endif
