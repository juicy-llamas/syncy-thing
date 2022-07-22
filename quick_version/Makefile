
BDIR=build
SDIR=src
TDIR=$(SDIR)/tests
CFLAGS=-c -Wall -O2 -Isrc/include

REQ_O = $(BDIR)/header-constants.o

testnet: $(BDIR)/net_one.o
	$(CC) $^ -o $@

app: $(BDIR)/main.o $(BDIR)/config.o $(REQ_O)
	$(CC) $^ -o $@

tgaddr: $(BDIR)/test_getaddrinfo.o
	$(CC) $^ -o $@

tnet: $(BDIR)/main-testnetfns.o $(BDIR)/net-help.o
	$(CC) $^ -o $@

test: $(REQ_O)
	@num_t=0
	@num_s=0
	@for i in (ls $(TDIR)); do					\
		echo "test $i"						\
		$(CC) $i $(CFLAGS) -o ${i%%.c}.o			\
		$(CC) ${i%%.c}.o $^ -o tmp				\
		./tmp							\
		[[ $? == 0 ]] 						\
			&& { echo "success"; (( num_s += 1 ))		\
			|| echo "failure"				\
		(( num_t += 1 ))					\
		rm ${i%%.c}.o						\
	done

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean
clean:
	$(RM) $(BDIR)/*
