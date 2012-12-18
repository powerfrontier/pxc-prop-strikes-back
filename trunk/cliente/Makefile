INCLUDEPATH = include/
SRCPATH = src/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -I../conexion/$(INCLUDEPATH) -Igame/$(INCLUDEPATH) -std=c++0x
LDFLAGS = -L$(BINPATH) -lConnection -lGame -lssl -lcrypto -pthread -lSDL

.PHONY: all clean cleanest $(BINPATH)libConnection.a game/$(BINPATH)libGame.a

all: $(BINPATH)client

$(BINPATH)client: 	$(BINPATH)libConnection.a \
			$(BINPATH)libGame.a \
			$(BINPATH)Client.o \
			$(BINPATH)ClientProfile.o \
			$(BINPATH)InstLogin.o \
			$(BINPATH)InstRouter.o \
			$(BINPATH)InstServer.o \
			$(BINPATH)main.o

	$(CC) -o $@ $^ $(LDFLAGS)

$(BINPATH)libConnection.a:
	$(MAKE) -C ../conexion/
	cp ../conexion/bin/libConnection.a $(BINPATH)

$(BINPATH)libGame.a:
	$(MAKE) -C game/
	cp game/bin/libGame.a $(BINPATH)

$(BINPATH)Client.o: $(SRCPATH)Client.cpp $(INCLUDEPATH)Client.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)ClientProfile.o: $(SRCPATH)ClientProfile.cpp $(INCLUDEPATH)ClientProfile.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)InstLogin.o: $(SRCPATH)InstLogin.cpp $(INCLUDEPATH)InstLogin.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)InstRouter.o: $(SRCPATH)InstRouter.cpp $(INCLUDEPATH)InstRouter.h
	$(CC) -c $(CFLAGS) $< -o $@
	
$(BINPATH)InstServer.o: $(SRCPATH)InstServer.cpp $(INCLUDEPATH)InstServer.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)main.o: $(SRCPATH)main.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libConnection.a
	rm -f $(BINPATH)libGame.a

cleanest: clean
	rm -f $(BINPATH)client

