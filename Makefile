# Makefile for MaTest

CC = cc
OBJS = MaTest.o connectives.o user.o wffs_polk.o

all : MaTest

MaTest : $(OBJS)
	$(CC) $(OBJS) -o MaTest

$(OBJS) : MaTest.h

