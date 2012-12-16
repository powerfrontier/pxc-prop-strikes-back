INCLUDEPATH = include/
SRCPATH = src/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -I../conexion/$(INCLUDEPATH) -std=c++0x
LDFLAGS = -L$(BINPATH) -lConnection -lssl -lcrypto -pthread

.PHONY: all clean cleanest $(BINPATH)libConnection.a game/$(BINPATH)libGame.a

all: $(BINPATH)router

$(BINPATH)router: 	$(BINPATH)libConnection.a \
			$(BINPATH)Router.o \
			$(BINPATH)RouterProfile.o \
			$(BINPATH)User.o \
			$(BINPATH)Zone.o \
			$(BINPATH)Server.o \
			$(BINPATH)InstControl.o \
			$(BINPATH)main.o

	$(CC) -o $@ $^ $(LDFLAGS)

$(BINPATH)libConnection.a:
	$(MAKE) -C ../conexion/
	cp ../conexion/bin/libConnection.a $(BINPATH)

$(BINPATH)Router.o: $(SRCPATH)Router.cpp $(INCLUDEPATH)Router.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)RouterProfile.o: $(SRCPATH)RouterProfile.cpp $(INCLUDEPATH)Router.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)User.o: $(SRCPATH)User.cpp $(INCLUDEPATH)Router.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)Zone.o: $(SRCPATH)Zone.cpp $(INCLUDEPATH)Router.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)Server.o: $(SRCPATH)Server.cpp $(INCLUDEPATH)Router.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)InstControl.o: $(SRCPATH)InstControl.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)main.o: $(SRCPATH)main.cpp
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libConnection.a

cleanest: clean
	rm -f $(BINPATH)gameServer

