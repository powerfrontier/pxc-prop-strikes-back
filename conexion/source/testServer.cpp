#include <string>
#include "Datagram.h"
#include "ConnectionManager.h"
#include "Connection.h"
#include <iostream>

int main(int argc, char** argv){
	printf("Main: Primera instruccion\n");
 	fflush(stdout);
	ConnectionManager* n = new ConnectionManager();
	printf("Main: Objeto creado\n");
 	fflush(stdout);
	n->listen(std::string(argv[1]));
	printf("Main: Estoy en listen\n");
 	fflush(stdout);
	while (1){
	}
}


