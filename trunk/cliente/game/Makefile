INCLUDEPATH = include/
SRCPATH = src/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -I../$(INCLUDEPATH) -I../../conexion/$(INCLUDEPATH) -std=c++0x

.PHONY: all clean cleanest

all: $(BINPATH)libGame.a

$(BINPATH)libGame.a: 	$(BINPATH)PXCGame.o \
			$(BINPATH)PXCZone.o
	ar rcs $@ $^

$(BINPATH)PXCGame.o: $(SRCPATH)PXCGame.cpp $(INCLUDEPATH)PXCGame.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)PXCZone.o: $(SRCPATH)PXCZone.cpp $(INCLUDEPATH)PXCGame.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINPATH)*.o

cleanest: clean
	rm -f $(BINPATH)libGame.a
