# Makefile for MaTest

# Where the binaries will go with install
BIN = /usr/local/bin

# Compiler configuration
CC = cc
OPTIMIZATION = -O3 -pipe -ftracer -fomit-frame-pointer -fPIC
#OPTIMIZATION = -g -Wall
SYSTEM = POSIX
#SYSTEM = WIN
VERBOSITY = -W -Wall -ansi -pedantic
CFLAGS = $(OPTIMIZATION) $(VERBOSITY) -D$(SYSTEM)

OBJS = MaTest.o connectives.o wffs_pn.o variables.o user.o evaluation.o

all : MaTest
	
MaTest : $(OBJS)
	$(CC) $(OBJS) -o matest

install : all
	mkdir -p $(BIN)
	cp matest $(BIN)

clean :
	rm -f *.o matest

uninstall :
	rm -f $(BIN)/matest

$(OBJS) : MaTest.h logics.h
