INCLUDEPATH = include/
INCLUDECPPCON = include/cppcon/

SRCPATH = src/
BINPATH = bin/
CC      = g++
CFLAGS  = -I$(INCLUDEPATH) -I../conexion/$(INCLUDEPATH) -I../sqlConnector/$(INCLUDEPATH) -I../sqlConnector/$(INCLUDECPPCON) -I../mysqlDriver/$(INCLUDEPATH) -std=c++0x
LDFLAGS = -L$(BINPATH) -lConnection -lssl -lcrypto -pthread -lmysqlclient -lpthread


all: $(BINPATH)Login

$(BINPATH)Login: $(BINPATH)libConnection.a $(BINPATH)libmysqlclient.a $(BINPATH)LoginRequest.o $(BINPATH)LogoutRequest.o $(BINPATH)ControlConnection.o $(BINPATH)LoginProfile.o $(BINPATH)Login.o $(BINPATH)main.o 
	$(CC) -o $@ $^ $(LDFLAGS) 

$(BINPATH)libConnection.a:
	$(MAKE) -C ../conexion/
	cp ../conexion/bin/libConnection.a $(BINPATH)


$(BINPATH)libmysqlclient.a:
	cp ../mysqlDriver/lib/libmysqlclient.a $(BINPATH)

$(BINPATH)Login.o: $(SRCPATH)Login.cpp $(INCLUDEPATH)Login.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)LoginRequest.o: $(SRCPATH)LoginRequest.cpp $(INCLUDEPATH)LoginRequest.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)LogoutRequest.o: $(SRCPATH)LogoutRequest.cpp $(INCLUDEPATH)LogoutRequest.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)ControlConnection.o: $(SRCPATH)ControlConnection.cpp $(INCLUDEPATH)ControlConnection.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)LoginProfile.o: $(SRCPATH)LoginProfile.cpp $(INCLUDEPATH)LoginProfile.h
	$(CC) -c $(CFLAGS) $< -o $@

$(BINPATH)main.o: $(SRCPATH)main.cpp 
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -f $(BINPATH)*.o
	rm -f $(BINPATH)libConnection.a
	rm -f $(BINPATH)libmysqlcppconn-static.a

cleanest: clean
	rm -f $(BINPATH)Login

.PHONY: all clean cleanest $(BINPATH)libConnection.a

