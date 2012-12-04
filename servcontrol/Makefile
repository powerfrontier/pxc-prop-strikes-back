Done: Control
	rm *.o
	echo "DONE"

Control: Connection.o ConnectionException.o TCPConnection.o Transferable.o TransferableFactory.o ServerLoad.o Server.o ControlProfile.o Control.o
	g++ ConnectionException.o Connection.o TCPConnection.o TransferableFactory.o Transferable.o ServerLoad.o Server.o ControlProfile.o Control.o -o Control -lssl -lcrypto -pthread -std=c++0x

ServerLoad.o: ./src/ServerLoad.cpp
	g++ -c ./src/ServerLoad.cpp -I./ -I../conexion/include -I./include -std=c++0x

Server.o: ./src/Server.cpp
	g++ -c ./src/Server.cpp -I./ -I../conexion/include -I./include -std=c++0x

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
Control.o: ./src/Control.cpp
	g++ -c ./src/Control.cpp -I./ -I../conexion/include -I./include -std=c++0x

ControlProfile.o: ./src/ControlProfile.cpp
	g++ -c ./src/ControlProfile.cpp -I../conexion/include -I./include -std=c++0x

clean:
	rm Control
