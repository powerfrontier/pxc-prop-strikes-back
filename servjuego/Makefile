INCLUDEPATH = include/
SRCPATH = src/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -I../conexion/$(INCLUDEPATH) -std=c++0x
LDFLAGS = -lssl -lcrypto -lConnection -L$(BINPATH)

all: $(BINPATH)gameServer

$(BINPATH)gameServer: $(BINPATH)libConnection.a $(BINPATH)GameServer.o $(BINPATH)GameServerProfile.o $(BINPATH)main.o $(BINPATH)Zone.o $(BINPATH)ZoneHandler.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BINPATH)libConnection.a:
	$(MAKE) -C ../conexion/
	cp ../conexion/bin/libConnection.a $(BINPATH)

$(BINPATH)GameServer.o: $(SRCPATH)GameServer.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)GameServerProfile.o: $(SRCPATH)GameServerProfile.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)main.o: $(SRCPATH)main.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)Zone.o: $(SRCPATH)Zone.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)ZoneHandler.o: $(SRCPATH)ZoneHandler.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libConnection.a

cleanest: clean
	rm -f $(BINPATH)gameServer

.PHONY: clean cleanest