
BDIR=build
SDIR=src
TDIR=src/tests
CFLAGS=-c -Wall -O2 -Isrc/include

ALL_S = ${wildcard $(SDIR)/*.c}
ALL_O = ${patsubst $(SDIR)/%,$(BDIR)/%,${ALL_S:.c=.o}}
REQ_O = $(ALL_O)

test_app: $(REQ_O)
	$(CC) $(REQ_O) -O2 -o test_app

app: $(REQ_O)
	$(CC) $(REQ_O) -O2 -o app

test: $(REQ_O) clean
	@num_t=0
	@num_s=0
	@for i in $$(ls $(TDIR)); do					\
		echo "test $i";						\
		$(CC) $i $(CFLAGS) -o ${i%%.c}.o;			\
		$(CC) ${i%%.c}.o $^ -o tmp;				\
		./tmp;							\
		[[ $? == 0 ]] 						\
			&& { echo "success"; (( num_s += 1 ))		\
			|| echo "failure";				\
		(( num_t += 1 ));					\
		rm ${i%%.c}.o;						\
	done;

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean
clean:
	$(RM) app
	$(RM) $(BDIR)/*
