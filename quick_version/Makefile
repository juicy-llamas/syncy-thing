
BDIR=build
SDIR=src
TDIR=$(SDIR)/tests
CFLAGS=-c -Wall -O2 -Isrc/include

testnet: $(BDIR)/net_one.o
	$(CC) $? -o $@

app: $(BDIR)/main.o $(BDIR)/config.o
	$(CC) $? -o $@

tgaddr: $(BDIR)/test_getaddrinfo.o
	$(CC) $? -o $@

tnet: $(BDIR)/main-testnetfns.o $(BDIR)/net-help.o
	$(CC) $? -o $@

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $? $(CFLAGS) -o $@

.PHONY: clean
clean:
	$(RM) $(BDIR)/*
