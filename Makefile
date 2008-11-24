# Makefile for MaTest

CC = cc -D$(SYSTEM) $(VERBOSITY)
OBJS = MaTest.o connectives.o wffs_pn.o variables.o user.o evaluation.o
SYSTEM = UNIX
#SYSTEM = WIN
VERBOSITY = -ansi

all : MaTest

MaTest : $(OBJS)
	$(CC) $(OBJS) -o MaTest

$(OBJS) : MaTest.h

