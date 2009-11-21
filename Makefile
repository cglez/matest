# Makefile for MaTest

# Where the binaries will be installed
BIN = /usr/local/bin

# Compiler configuration
CC = cc $(DEBUG) $(LIBS) $(CFLAGS) $(OPTIMIZATION)
VERBOSITY = -W -ansi -pedantic
CFLAGS = -export-dynamic `pkg-config --cflags libglade-2.0`
LIBS = `pkg-config --libs libglade-2.0`
DEBUG  = -Wall -g
OPTIMIZATION = -O3 -pipe -ftracer -fomit-frame-pointer -fPIC
SYSTEM = POSIX
#SYSTEM = WIN32

OBJECTS = src/MaTest.o src/connectives.o src/wffs_pn.o src/variables.o \
          src/user.o src/user_text.o src/user_gui.o src/evaluation.o \
          src/callbacks.c

all : MaTest
	
MaTest : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

install : all
	mkdir -p $(BIN)
	cp matest $(BIN)

clean :
	rm -f src/*.o matest

uninstall :
	rm -f $(BIN)/matest

$(OBJECTS) : src/MaTest.h src/logics.h
