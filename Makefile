# Makefile for MaTest

CC = cc
OBJS = MaTest.o connectives.o wffs_polk.o variables.o
OSDEP_OBJS = user.o
OSDEP_SRC = user.c
SYSTEM = UNIX
#SYSTEM = WIN

all : MaTest

MaTest : $(OBJS) $(OSDEP_OBJS)
	$(CC) $(OBJS) $(OSDEP_OBJS) -o MaTest

$(OBJS) : MaTest.h
$(OSDEP_OBJS) : MaTest.h
	$(CC) -D$(SYSTEM) -c $(OSDEP_SRC) -o $(OSDEP_OBJS)

