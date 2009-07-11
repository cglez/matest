# Makefile for MaTest

# Where the binaries will be installed
BIN = /usr/local/bin

# Compiler configuration
CC = cc
OPTIMIZATION = -O3 -pipe -ftracer -fomit-frame-pointer -fPIC
#OPTIMIZATION = -g -Wall
SYSTEM = POSIX
#SYSTEM = WIN32
VERBOSITY = -W -Wall -ansi
CFLAGS = $(OPTIMIZATION) $(VERBOSITY) -D$(SYSTEM)

OBJS = src/MaTest.o src/connectives.o src/wffs_pn.o src/variables.o src/user.o src/evaluation.o

all : MaTest
	
MaTest : $(OBJS)
	$(CC) $(OBJS) -o matest

install : all
	mkdir -p $(BIN)
	cp matest $(BIN)

clean :
	rm -f src/*.o matest

uninstall :
	rm -f $(BIN)/matest

$(OBJS) : src/MaTest.h src/logics.h
