INCLUDEPATH = include/
SRCPATH = src/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -I../conexion/$(INCLUDEPATH) -std=c++0x
LDFLAGS = -L$(BINPATH) -lConnection -lGame -lssl -lcrypto -pthread

.PHONY: all clean cleanest $(BINPATH)libConnection.a game/$(BINPATH)libGame.a

all: $(BINPATH)gameServer

$(BINPATH)gameServer: 	$(BINPATH)libConnection.a \
			$(BINPATH)libGame.a \
			$(BINPATH)GameServer.o \
			$(BINPATH)GameServerProfile.o \
			$(BINPATH)main.o \
			$(BINPATH)Zone.o \
			$(BINPATH)ZoneHandler.o \
			$(BINPATH)InstControl.o \
			$(BINPATH)InstRouter.o

	$(CC) -o $@ $^ $(LDFLAGS)

$(BINPATH)libConnection.a:
	$(MAKE) -C ../conexion/
	cp ../conexion/bin/libConnection.a $(BINPATH)

$(BINPATH)libGame.a:
	$(MAKE) -C game/
	cp game/bin/libGame.a $(BINPATH)

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

$(BINPATH)InstControl.o: $(SRCPATH)InstControl.cpp $(INCLUDEPATH)InstControl.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)InstRouter.o: $(SRCPATH)InstRouter.cpp $(INCLUDEPATH)InstRouter.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libConnection.a
	rm -f $(BINPATH)libGame.a

cleanest: clean
	rm -f $(BINPATH)gameServer

