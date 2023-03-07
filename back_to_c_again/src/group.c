


struct group {
	unsigned long int flags;
	FILE* files;
	struct peers* peers;
	unsigned int fileslen;
	unsigned int peerslen;
};
