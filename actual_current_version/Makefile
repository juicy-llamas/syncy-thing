
.PHONY: test clean

# ======
# Config
# ======

# are we debugging (MUST BE EQUAL TO "true" for debugging to work!)
DEBUG=true

# directories start from makefile directory
BDIR=build
SDIR=src
IDIR=src/include
TDIR=src/tests
# stands for IGNORE_FILES
# if files are in a directory with this name, just ignore them (don't build or run them)
# note: include files in directories of this name can still be explicitly included by specifiying the path.
# also, files on the makefile root directory are just ignored by default, since they aren't in an 'action' directory
IGNFLS=__ignore__
# entry is the file with main, again specified from the root of the Makefile to prevent ambiguity
# it's important because it's NOT linked with the tests
ENTRY=src/main.c
APPNAME=app

# you can set your various flags here
base_flags=-c -Wall -Wextra -I$(IDIR)
base_lflags=-fstack-protector-strong -lcrypto -lssl -lpthread

debug_flags=-g -D'DEBUG'
debug_lflags=-g

build_flags=-fPIE -O3
build_lflags=-Wl,-z,noexecstack -pie -fPIE

# =================================================
# Logic to select the right files and build options
# =================================================

# this script gets the correct object files while simultaneously making the directory structure
# assume that every source file is tested, EXCEPT for main b.c. multiple entry point issue
TEST_O=$(shell for i in $$(find $(SDIR) ! -path "$(IDIR)/*" ! -path "$(TDIR)/*" ! -path "*/$(IGNFLS)/*"	\
	-type f); do [ $$i = "$(ENTRY)" ] && continue; tmp=$${i#src/}; fl=$${tmp%.c}; pth=$${tmp%/};	\
	mkdir -p "build/$$pth"; echo "$(BDIR)/$$fl.o"; done;)
# add main back in. Theoretically, with some changes, you could have multiple targets that use different objects.
APP_O=$(TEST_O) $(BDIR)/$(ENTRY).o

ifeq '$(DEBUG)' 'true'
	CFLAGS=$(debug_flags) $(base_flags)
	LFLAGS=$(debug_lflags) $(base_lflags)
else
	CFLAGS=$(build_flags) $(base_flags)
	LFLAGS=$(build_lflags) $(base_lflags)
endif

# ==============================================
# This is where all of the actual make rules are
# ==============================================

$(APPNAME): $(APP_O)
	mkdir -p $(BDIR)
	$(CC) $^ -o $@ $(LFLAGS)

$(BDIR)/%.o: $(SDIR)/%.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM) -r $(BDIR)
	$(RM) app
	$(RM) test_log
	mkdir $(BDIR)

# ============================================
# Just a neat little automated testing utility
# ============================================

# The testing utility will dump results to stdout and to a test_log, which WILL get overwritten
# for each tests (fairly simple to change this, if anyone wants or cares)
# Also, to skip tests, just put them in an __ignore__ dir (or whatever name you specified)
test: $(TEST_O)
	@num_t=0;										\
	num_s=0;										\
	echo "" >> test_log;									\
	echo "" >> test_log;									\
	mkdir -p "$(BDIR)/$(TDIR)"								\
 	for i in $$(find $(TDIR) ! -path "*/$(IGNFLS)/*" -type f); do				\
		echo "========================================" | tee -a test_log;		\
		echo "	test $$i" | tee -a test_log;						\
		echo "========================================" | tee -a test_log;		\
		$(CC) $$i -o $(BDIR)/$${i%%.c}.o $(CFLAGS);					\
		[ $$? -ne 0 ] &&								\
			{ $(CC) $$$i -o $(BDIR)/$${i%%.c}.o $(CFLAGS) >> test_log 2>&1;		\
			  echo "" | tee -a test_log;						\
			  printf "Compilation failed\n\n" | tee -a test_log; continue; };	\
		$(CC) $^ $(BDIR)/$${i%%.c}.o -o cur_test $(LFLAGS);				\
		[ $$? -ne 0 ] && 								\
			{ echo "Linking failed" >> test_log; 					\
			  continue; };								\
		./cur_test 2>&1 | tee -a test_log;						\
		if [ $$? -eq 0 ]; then 								\
			echo "Success" | tee -a test_log;					\
			num_s=$$(( num_s+=1 ));							\
		else										\
			echo "Failure" | tee -a test_log;					\
		fi;										\
		num_t=$$(( num_t+=1 ));								\
	done;											\
	rm -f cur_test;										\
	echo "========================================" | tee -a test_log;			\
	echo "	conclusions" | tee -a test_log;							\
	echo "========================================" | tee -a test_log;			\
	if [ $$num_s -eq $$num_t ]; then							\
		echo "$$num_t test(s) succeeded" | tee -a test_log;				\
	else											\
		echo "$$(( num_t-num_s )) out of $$num_t tests failed" | tee -a test_log;	\
	fi;											\
	echo "" >> test_log;									\
	echo "" >> test_log;
