#include <string>
#include "Datagram.h"
#include "Connection.h"
#include <TestClasses.h>
#include <iostream>

int main(int argc, char** argv){
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new TestProfile());
	TransferableFactory::instance().setProtocol("0.1a");

	printf("MainClient(): 1\n");
        fflush(stdout);
	Connection* n = new TCPConnection();
	printf("MainClient(): objeto Conexion creado\n");
        fflush(stdout);
	n->connect(std::string(argv[1]),std::string(argv[2]));
	printf("MainClient(): Connected\n");
        fflush(stdout);

	TestTransferableSent* sent = NULL;
	sent = new TestTransferableSent();
	sent->m1(5);
	n->send(*sent);
	while (1){

	}
}


