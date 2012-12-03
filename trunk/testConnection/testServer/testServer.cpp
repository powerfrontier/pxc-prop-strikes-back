#include <string>
#include "Datagram.h"
#include "ConnectionManager.h"
#include "Connection.h"
#include <iostream>
#include <TestClasses.h>
#include <iostream>




int main(int argc, char** argv){
	//Esto se hace al principio del main (inicialiación de la conexión)
	//TestProfile es herenccia de TransferableProfile para decirle 
	TransferableFactory::instance().setProfile(new TestProfile());
	TransferableFactory::instance().setProtocol("0.1a");


	printf("MainServer: Primera instruccion\n");
 	fflush(stdout);
	ConnectionManager* n = new ConnectionManager();
	printf("MainServer: Objeto ConnectionManager creado\n");
 	fflush(stdout);
	n->listen(std::string(argv[1]));
	printf("MainServer: Estoy en listen\n");
 	fflush(stdout);
	while (1){}
}


