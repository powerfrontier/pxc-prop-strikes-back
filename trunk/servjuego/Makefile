INCLUDEPATH = include/
SRCPATH = src/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -I../conexion/$(INCLUDEPATH) -std=c++0x
LDFLAGS = -lssl -lcrypto -lConnection -L$(BINPATH) -pthread

all: $(BINPATH)gameServer

$(BINPATH)gameServer: $(BINPATH)libConnection.a $(BINPATH)GameServer.o $(BINPATH)GameServerProfile.o $(BINPATH)main.o $(BINPATH)Zone.o $(BINPATH)ZoneHandler.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BINPATH)libConnection.a:
	$(MAKE) -C ../conexion/
	cp ../conexion/bin/libConnection.a $(BINPATH)

$(BINPATH)GameServer.o: $(SRCPATH)GameServer.cpp $(INCLUDEPATH)GameServer.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)GameServerProfile.o: $(SRCPATH)GameServerProfile.cpp $(INCLUDEPATH)GameServerProfile.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)main.o: $(SRCPATH)main.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)Zone.o: $(SRCPATH)Zone.cpp $(INCLUDEPATH)Zone.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)ZoneHandler.o: $(SRCPATH)ZoneHandler.cpp $(INCLUDEPATH)Zone.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libConnection.a

cleanest: clean
	rm -f $(BINPATH)gameServer

.PHONY: all clean cleanest $(BINPATH)libConnection.a