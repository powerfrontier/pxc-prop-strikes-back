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
	if(n->connect(std::string(argv[1]),std::string(argv[2]))) { 
		printf("MainClient(): objeto Conexion creado\n");
        	fflush(stdout);
		printf("MainClient(): Connected\n");
        	fflush(stdout);

		TestTransferableSent* sent = NULL;
		sent = new TestTransferableSent();
		int i = 0;
		sent->m1(i);
		printf("MainClient(): Sending...\n");
		fflush(stdout);
		n->send(*sent);
		printf("MainClient(): sent\n");
       		fflush(stdout);
		while (1){
		}
	}else{
		std::cout << "NoooooooooooooooooooooooOO!" << std::endl;
	}
}


