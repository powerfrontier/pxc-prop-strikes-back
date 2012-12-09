Done: testClient.exe testServer.exe
	echo "DONE"

testClient.exe: testClient.o Connection.o ConnectionException.o TCPConnection.o Transferable.o TransferableFactory.o TestClasses.o TransferableProfile.o
	g++ TestClasses.o ConnectionException.o Connection.o TCPConnection.o testClient.o TransferableFactory.o Transferable.o TransferableProfile.o -o testClient.exe -lssl -lcrypto -pthread

testServer.exe: testServer.o Connection.o ConnectionException.o TCPConnection.o Transferable.o TransferableFactory.o ConnectionManager.o TestClassesServer.o TransferableProfile.o
	g++ TestClassesServer.o ConnectionException.o Connection.o TCPConnection.o testServer.o TransferableFactory.o Transferable.o TransferableProfile.o ConnectionManager.o -o testServer.exe -lssl -lcrypto -pthread
TestClasses.o: testClasses/TestClasses.cpp
	g++ -c testClasses/TestClasses.cpp -I../conexion/include/ -I./testClasses/ -std=c++0x
TestClassesServer.o: testClasses/TestClassesServer.cpp
	g++ -c testClasses/TestClassesServer.cpp -I../conexion/include/ -I./testClasses/ -std=c++0x
testServer.o: testServer/testServer.cpp
	g++ -c testServer/testServer.cpp -I../conexion/include/ -I./testClasses/ -std=c++0x
testClient.o: testClient/testClient.cpp
	g++ -c testClient/testClient.cpp -I../conexion/include/ -I./testClasses/ -std=c++0x
ConnectionManager.o: ../conexion/source/ConnectionManager.cpp
	g++ -c ../conexion/source/ConnectionManager.cpp -I../conexion/include -std=c++0x
Connection.o: ../conexion/source/Connection.cpp
	g++ -c ../conexion/source/Connection.cpp -I../conexion/include/ -std=c++0x
ConnectionException.o: ../conexion/source/ConnectionException.cpp
	g++ -c ../conexion/source/ConnectionException.cpp -I../conexion/include/ -std=c++0x
TCPConnection.o: ../conexion/source/TCPConnection.cpp
	g++ -c ../conexion/source/TCPConnection.cpp -I../conexion/include/ -std=c++0x
Transferable.o: ../conexion/source/Transferable.cpp
	g++ -c ../conexion/source/Transferable.cpp -I../conexion/include/ -std=c++0x
TransferableProfile.o: ../conexion/source/TransferableProfile.cpp
	g++ -c  ../conexion/source/TransferableProfile.cpp -I../conexion/include/ -std=c++0x
TransferableFactory.o: ../conexion/source/TransferableFactory.cpp
	g++ -c ../conexion/source/TransferableFactory.cpp -I../conexion/include/ -std=c++0x

clean:
	rm *.o
	rm *.exe
