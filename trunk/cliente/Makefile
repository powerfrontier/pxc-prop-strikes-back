# Generated automatically from Makefile.in by configure.
CC=gcc
CFLAGS=-g -I/usr/include  -Wall 
LD=-L/usr/lib  -lssl -lcrypto

OBJS=common.o 

all:  clientMain

clientMain: clientMain.o client.o read_write.o $(OBJS)
	$(CC) clientMain.o client.o read_write.o $(OBJS) -o clientMain $(LD)

clean:	
	rm *.o clientMain

