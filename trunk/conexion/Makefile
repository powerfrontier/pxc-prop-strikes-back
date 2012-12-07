INCLUDEPATH = include/
SRCPATH = source/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -std=c++0x
LDFLAGS = -lssl -lcrypto

all: ConnectionException.o Connection.o ConnectionManager.o TCPConnection.o Transferable.o TransferableFactory.o TransferableProfile.o

ConnectionException.o: $(SRCPATH)ConnectionException.cpp
	$(CC) -c $(CFLAGS) $< -o $(BINPATH)$@

Connection.o: $(SRCPATH)Connection.cpp
	$(CC) -c $(CFLAGS) $< -o $(BINPATH)$@

ConnectionManager.o: $(SRCPATH)ConnectionManager.cpp
	$(CC) -c $(CFLAGS) $< -o $(BINPATH)$@

TCPConnection.o: $(SRCPATH)TCPConnection.cpp
	$(CC) -c $(CFLAGS) $< -o $(BINPATH)$@

Transferable.o: $(SRCPATH)Transferable.cpp
	$(CC) -c $(CFLAGS) $< -o $(BINPATH)$@

TransferableFactory.o: $(SRCPATH)TransferableFactory.cpp
	$(CC) -c $(CFLAGS) $< -o $(BINPATH)$@

TransferableProfile.o: $(SRCPATH)TransferableProfile.cpp
	$(CC) -c $(CFLAGS) $< -o $(BINPATH)$@


clean:
	rm -f bin/*.o
