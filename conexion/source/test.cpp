#include <string>
#include "Datagram.h"
#include "Connection.h"

int main(int argc, char** argv){
	Connection* n = new TCPConnection();
	n->connect(std::string(argv[1]),std::string(argv[2]));
	n->close();
}


