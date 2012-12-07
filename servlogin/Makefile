Done: Login
	echo "DONE"

Login: Connection.o ConnectionException.o TCPConnection.o Transferable.o TransferableFactory.o TransferableProfile.o Login.o
	g++ ConnectionException.o Connection.o TCPConnection.o Transferable.o TransferableFactory.o  TransferableProfile.o Login.o -o Login -lssl -lcrypto -pthread -std=c++0x


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
TransferableFactory.o: ../conexion/source/TransferableFactory.cpp
	g++ -c ../conexion/source/TransferableFactory.cpp -I../conexion/include/ -std=c++0x
TransferableProfile.o: ../conexion/source/TransferableProfile.cpp
	g++ -c ../conexion/source/TransferableProfile.cpp -I../conexion/include/ -std=c++0x
IPServer.o: ./src/IPServer.cpp
	g++ -c ./src/IPServer.cpp -I./ -I../conexion/include -I./include -std=c++0x
Login.o: ./src/Login.cpp
	g++ -c ./src/Login.cpp -I./ -I../conexion/include -I./include -std=c++0x



LoginProfile.o: ./src/Login.cpp
	g++ -c ./src/Login.cpp -I../conexion/include -I./include -std=c++0x

clean:
	rm Login
	rm *.o
