INCLUDEPATH = include/
SRCPATH = source/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -std=c++0x
LDFLAGS = -lssl -lcrypto

all: $(BINPATH)libConnection.a

$(BINPATH)libConnection.a: $(BINPATH)ConnectionException.o $(BINPATH)Connection.o $(BINPATH)ConnectionManager.o $(BINPATH)TCPConnection.o $(BINPATH)Transferable.o $(BINPATH)TransferableFactory.o $(BINPATH)TransferableProfile.o
	ar rcs $@ $^

$(BINPATH)ConnectionException.o: $(SRCPATH)ConnectionException.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)Connection.o: $(SRCPATH)Connection.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)ConnectionManager.o: $(SRCPATH)ConnectionManager.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)TCPConnection.o: $(SRCPATH)TCPConnection.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)Transferable.o: $(SRCPATH)Transferable.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)TransferableFactory.o: $(SRCPATH)TransferableFactory.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)TransferableProfile.o: $(SRCPATH)TransferableProfile.cpp
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -f $(BINPATH)*.o

cleanest: clean
	rm -f $(BINPATH)libConnection.a