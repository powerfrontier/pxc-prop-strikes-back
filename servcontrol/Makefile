Done: balanceo.exe
	rm *.o
	echo "DONE"

balanceo.exe: Connection.o ConnectionException.o TCPConnection.o Transferable.o TransferableFactory.o SendServerLoad.o SharedStructs.o
	g++ ConnectionException.o Connection.o TCPConnection.o TransferableFactory.o Transferable.o SendServerLoad.o SharedStructs.o balanceo.o -o balanceo.exe -lssl -lcrypto -pthread -std=c++0x



SendServerLoad.o: ./instrucciones/SendServerLoad.cpp
	g++ -c ./instrucciones/SendServerLoad.cpp -I./ -I../conexion/include -I./instrucciones -std=c++0x

SharedStructs.o: ./instrucciones/SharedStructs.cpp
	g++ -c ./instrucciones/SharedStructs.cpp -I./ -I../conexion/include -I./instrucciones -std=c++0x

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
balanceo.o: balanceo.cpp
	g++ -c balanceo.cpp -I./ -I../conexion/include -I./instrucciones -std=c++0x
clean:
	rm *.exe
