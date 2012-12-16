Done: testClient.exe 
	echo "DONE"

testClient.exe: testClient.o Connection.o ConnectionException.o TCPConnection.o Transferable.o TransferableFactory.o TestClasses.o TransferableProfile.o TCPConnectionSecurity.o sysConnect.o
	g++ TestClasses.o ConnectionException.o Connection.o TCPConnection.o testClient.o TransferableFactory.o Transferable.o TransferableProfile.o TCPConnectionSecurity.o sysConnect.o -o testClient -lssl -lcrypto -pthread

TestClasses.o: src/TestClasses.cpp
	g++ -c src/TestClasses.cpp -I../../conexion/include/ -I./testClasses/ -std=c++0x -I./include/
TestClassesServer.o: src/TestClassesServer.cpp
	g++ -c src/TestClassesServer.cpp -I../../conexion/include/ -I./testClasses/ -std=c++0x -I./include/
testClient.o: src/testClient.cpp
	g++ -c src/testClient.cpp -I../../conexion/include/ -I./testClasses/ -std=c++0x -I./include/
ConnectionManager.o: ../../conexion/source/ConnectionManager.cpp
	g++ -c ../../conexion/source/ConnectionManager.cpp -I../../conexion/include -std=c++0x
Connection.o: ../../conexion/source/Connection.cpp
	g++ -c ../../conexion/source/Connection.cpp -I../../conexion/include/ -std=c++0x
ConnectionException.o: ../../conexion/source/ConnectionException.cpp
	g++ -c ../../conexion/source/ConnectionException.cpp -I../../conexion/include/ -std=c++0x
TCPConnection.o: ../../conexion/source/TCPConnection.cpp
	g++ -c ../../conexion/source/TCPConnection.cpp -I../../conexion/include/ -std=c++0x
TCPConnectionSecurity.o: ../../conexion/source/TCPConnectionSecurity.cpp sysConnect.o
	g++ -c ../../conexion/source/TCPConnectionSecurity.cpp -I../../conexion/include/ -std=c++0x
sysConnect.o: ../../conexion/source/sysConnect.cpp 
	g++ -c ../../conexion/source/sysConnect.cpp -I../../conexion/include/ -std=c++0x
Transferable.o: ../../conexion/source/Transferable.cpp
	g++ -c ../../conexion/source/Transferable.cpp -I../../conexion/include/ -std=c++0x
TransferableProfile.o: ../../conexion/source/TransferableProfile.cpp
	g++ -c  ../../conexion/source/TransferableProfile.cpp -I../../conexion/include/ -std=c++0x
TransferableFactory.o: ../../conexion/source/TransferableFactory.cpp
	g++ -c ../../conexion/source/TransferableFactory.cpp -I../../conexion/include/ -std=c++0x


clean:
	rm *.o
	rm *.exe
