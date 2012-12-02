#include <string>
#include "Datagram.h"
#include "ConnectionManager.h"
#include "Connection.h"
#include <iostream>

int main(int argc, char** argv){
	char buff[64];
	str.copy(buff, str.size(), 0);
	printf(buff);
    printf("Ready");
    fflush(stdout);

	ConnectionManager* n = new ConnectionManager();
	str.copy(buff, str.size(), 0);
	printf(buff);
    printf("OBJETO ConnectionManager Creado");
    fflush(stdout);

	n->listen(std::string(argv[1]));
	str.copy(buff, str.size(), 0);
	printf(buff);
    printf("Listen");
    fflush(stdout);

	while (1){
	}
}


