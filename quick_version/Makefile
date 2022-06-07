
BDIR=build
SDIR=src
CFLAGS=-c -Wall -O2 -Isrc/include

testnet: $(BDIR)/net_one.o
	$(CC) $? -o $@

app: $(BDIR)/main.o
	$(CC) $? -o $@

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $? $(CFLAGS) -o $@

.PHONY: clean
clean:
	$(RM) $(BDIR)/*
