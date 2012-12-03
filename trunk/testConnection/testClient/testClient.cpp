#include <string>
#include "Datagram.h"
#include "Connection.h"

int main(int argc, char** argv){
	printf("MainClient(): 1\n");
        fflush(stdout);
	Connection* n = new TCPConnection();
	printf("MainClient(): objeto Conexion creado\n");
        fflush(stdout);
	n->connect(std::string(argv[1]),std::string(argv[2]));
	printf("MainClient(): Connected\n");
        fflush(stdout);
	
	while (1){
	}
}


