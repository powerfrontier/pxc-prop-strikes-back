#!/bin/bash
rm *.o

g++ -std=c++0x -I../include/ ../source/Connection.cpp -pthread -c
g++ -std=c++0x -I../include/ ../source/ConnectionManager.cpp -pthread -c
g++ -std=c++0x -I../include/ ../source/ConnectionException.cpp -c
g++ -std=c++0x -I../include/ ../source/TCPConnection.cpp -pthread -c
g++ -std=c++0x -I../include/ ../source/Transferable.cpp -c
g++ -std=c++0x -I../include/ ../source/TransferableFactory.cpp -c
g++ -std=c++0x -I../include/ ../source/TransferableProfile.cpp -c
